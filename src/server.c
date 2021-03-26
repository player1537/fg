#include "server.h"

#include "voxel_traversal.h"
#include "vec.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>

#include <microhttpd.h>
#include <zlib.h>
#include <pthread.h>
#include <zorder.h>

#include "base64.h"
#include "MAB/log.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <glad/glad.h>
#define GLAPIENTRY APIENTRY
#include <GL/osmesa.h>

struct attrib {
	GLuint64 size;
	union {
		GLuint64 rawtype;
		GLenum type;
	};
	GLuint64 count;
	union {
		GLbyte range[8];
		GLfloat frange[2];
		GLint irange[2];
		GLuint urange[2];
	};
	union {
		GLbyte *data;
		GLfloat *floats;
		GLint *ints;
		GLuint *uints;
	};
};

typedef struct partition_data {
	Vec_GLuint partitions;
	GLuint indexBuffer;
    GLuint count;
} PartitionData;

enum {
	ERROR_NONE = 0,
	ERROR_COMPILE_VERTEX_SHADER,
	ERROR_COMPILE_FRAGMENT_SHADER,
	ERROR_LINK_PROGRAM,
	NUM_ERRORS,
};

char *errorMessages[NUM_ERRORS] = {
	[ERROR_NONE] = "Render Successful",
	[ERROR_COMPILE_VERTEX_SHADER] = "Could not compile vertex shader",
	[ERROR_COMPILE_FRAGMENT_SHADER] = "Could not compile fragment shader",
	[ERROR_LINK_PROGRAM] = "Could not link program",
};

static volatile GLsizei _resolution;
static volatile GLfloat _x;
static volatile GLfloat _y;
static volatile GLfloat _z;
static GLchar *volatile _dataset;
static volatile GLuint _dcIdent;
static GLuint *volatile _dcIndex;
static GLfloat *volatile _dcMult;
static GLfloat *volatile _dcOffset;
static volatile GLfloat _dcMinMult;
static volatile GLfloat _dcMaxMult;
static GLchar *volatile _info;
static GLchar *volatile _vertexShaderSource;
static GLchar *volatile _fragmentShaderSource;
static GLubyte *volatile _image;
static volatile GLubyte volatile _error;
static pthread_mutex_t *_lock;
static pthread_barrier_t *_barrier;

static volatile GLuint _max_depth = 0;
static unsigned volatile  _max_res;
static PartitionData *_partition_cache;

void log_partition_cache(PartitionData *pdc){
    printf("tot: %u\n", _max_res*_max_res);
    for(int i = 0; i < _max_res*_max_res; ++i){
        PartitionData *pd = &(pdc[i]);

        if(pd->partitions.length == 0) continue;

        printf("idx: %d\n", i);
        printf("edgeCount: %lu\n", pd->partitions.length/2);
        printf("     ");
        for(int j = 0; j < pd->partitions.length; ++j){
            printf("%d,", pd->partitions.data[j]);
        }
        printf("\n---------------------------------------\n\n");
    }

}

void *render(void *v) {
	OSMesaContext context;
	GLuint vertexShader, fragmentShader, program, /*indexBuffer,*/ aNodeBuffers[16];
    unsigned long i, j;
	GLuint64 ncount, ecount;
	GLint rc, uTranslateX, uTranslateY, uScale, uNodeMins[16], uNodeMaxs[16], aNodeLocations[16];
	GLchar log[512];
	struct attrib nattribs[16], eattribs[16], *edges;
	FILE *f;
	enum { INIT_OSMESA, INIT_GRAPH, INIT_BUFFERS, INIT_PROGRAM, INIT_UNIFORMS, INIT_PARTITION, INIT_ATTRIBUTES, INIT_DC, INIT_INDEX_BUFFER, RENDER, WAIT } where;

	GLfloat x, y, z;
    int max_depth;
	GLchar *dataset, *vertexShaderSource, *fragmentShaderSource;
	GLboolean first;
	GLuint dcIdent, *dcIndex;
	GLfloat *dcMult, *dcOffset, dcMinMult, dcMaxMult;
	
	_partition_cache = NULL;	
	_resolution = 256;

	context = NULL;
	vertexShader = fragmentShader = program = 0;
	nattribs[0] = (struct attrib){ 0 };
	eattribs[0] = (struct attrib){ 0 };
	edges = malloc(sizeof(*edges));
	*edges = (struct attrib){ 0 };
	aNodeBuffers[0] = 0;
	//indexBuffer = 0;
	dcIdent = 0;
	dcIndex = NULL;
	dcMult = dcOffset = NULL;

	x = y = z = INFINITY;
	dataset = vertexShaderSource = fragmentShaderSource = NULL;
	first = GL_TRUE;

	// sync with main function
	pthread_barrier_wait(_barrier);
	goto wait_for_request;

	for (;;)
	switch (where) {
	case INIT_OSMESA:
	MAB_WRAP("create osmesa context") {
		if (context) OSMesaDestroyContext(context);
		context = OSMesaCreateContextExt(OSMESA_RGBA, 16, 0, 0, NULL);
		if(!context) {
			fprintf(stderr, "could not init OSMesa context\n");
			exit(1);
		}

		_image = malloc(4 * _resolution * _resolution);
		if (!_image) {
			fprintf(stderr, "could not allocate image buffer\n");
			exit(1);
		}

		rc = OSMesaMakeCurrent(context, _image, GL_UNSIGNED_BYTE, _resolution, _resolution);
		if(!rc) {
			fprintf(stderr, "could not bind to image buffer\n");
			exit(1);
		}

		if (!gladLoadGLLoader((GLADloadproc)OSMesaGetProcAddress)) {
			printf("gladLoadGL failed\n");
			exit(1);
		}
		printf("OpenGL %s\n", glGetString(GL_VERSION));
	}
	__attribute__((fallthrough));

	case INIT_GRAPH:
	MAB_WRAP("load graph") {
		if (nattribs[0].data)
		for (i=0; i<ncount; ++i)
		free(nattribs[i].data);

		MAB_WRAP("load node attributes") {
			char temp[512];
			snprintf(temp, sizeof(temp), "data/%s/nodes.dat", _dataset);

			f = fopen(temp, "r");
			fread(&ncount, sizeof(ncount), 1, f);

			for (i=0; i<ncount; ++i) {
				nattribs[i] = (struct attrib){ 0 };

				fread(&nattribs[i].size, sizeof(nattribs[i].size), 1, f);

				fread(&nattribs[i].rawtype, sizeof(nattribs[i].rawtype), 1, f);
				switch (nattribs[i].rawtype) {
				case 'f': nattribs[i].type = GL_FLOAT; break;
				case 'i': nattribs[i].type = GL_INT; break;
				case 'u': nattribs[i].type = GL_UNSIGNED_INT; break;
				}

				fread(&nattribs[i].count, sizeof(nattribs[i].count), 1, f);

				fread(nattribs[i].range, sizeof(nattribs[i].range), 1, f);

				nattribs[i].data = malloc(nattribs[i].size);
				fread(nattribs[i].data, 1, nattribs[i].size, f);
			}

			fclose(f);
		}

		if (eattribs[0].data)
		for (i=0; i<ecount; ++i)
		free(eattribs[i].data);


		MAB_WRAP("load edge data") {
			char temp[512];
			snprintf(temp, sizeof(temp), "data/%s/edges.dat", _dataset);

			f = fopen(temp, "r");
			fread(&ecount, sizeof(ecount), 1, f);

			for (i=0; i<ecount; ++i) {
				eattribs[i] = (struct attrib){ 0 };

				fread(&eattribs[i].size, sizeof(eattribs[i].size), 1, f);

				fread(&eattribs[i].rawtype, sizeof(eattribs[i].rawtype), 1, f);
				switch (eattribs[i].rawtype) {
				case 'f': eattribs[i].type = GL_FLOAT; break;
				case 'i': eattribs[i].type = GL_INT; break;
				case 'u': eattribs[i].type = GL_UNSIGNED_INT; break;
				}

				fread(&eattribs[i].count, sizeof(eattribs[i].count), 1, f);

				fread(eattribs[i].range, sizeof(eattribs[i].range), 1, f);

				eattribs[i].data = malloc(eattribs[i].size);
				fread(eattribs[i].data, 1, eattribs[i].size, f);
			}

			fclose(f);
		}
	}
	__attribute__((fallthrough));
	
	case INIT_DC:
	MAB_WRAP("init index data") {
		GLfloat *dcDepth;
		dcDepth = malloc(eattribs[0].count / 2 * sizeof(*dcDepth));
		for (i=0; i<eattribs[0].count/2; ++i) {
			dcDepth[i] = 0.0;
			for (j=0; j<16; ++j) {
				if (!dcIndex[j]) break;
				float sourceDepth, targetDepth;

				sourceDepth = dcMult[j] * nattribs[dcIndex[j]-1].floats[eattribs[0].uints[2*i+0]] + dcOffset[j];
				targetDepth = dcMult[j] * nattribs[dcIndex[j]-1].floats[eattribs[0].uints[2*i+1]] + dcOffset[j];

				dcDepth[i] += sourceDepth < targetDepth
					? dcMinMult * sourceDepth + dcMaxMult * targetDepth
					: dcMinMult * targetDepth + dcMaxMult * sourceDepth;
			}
		}

		GLuint64 *dcReorder;
		dcReorder = malloc(eattribs[0].count / 2 * sizeof(*dcReorder));
		for (i=0; i<eattribs[0].count/2; ++i) {
			dcReorder[i] = i;
		}

		int compare(const void *av, const void *bv) {
			const GLuint64 *a = av;
			const GLuint64 *b = bv;

			if (dcDepth[*a] < dcDepth[*b]) return -1;
			if (dcDepth[*a] > dcDepth[*b]) return 1;
			return 0;
		}

		qsort(dcReorder, eattribs[0].count/2, sizeof(*dcReorder), compare);

		if (edges->data) free(edges->data);

		*edges = eattribs[0];
		edges->data = malloc(edges->size);
		for (i=0; i<eattribs[0].count/2; ++i) {
			edges->uints[2*i+0] = eattribs[0].uints[2*dcReorder[i]+0];
			edges->uints[2*i+1] = eattribs[0].uints[2*dcReorder[i]+1];
		}

		free(dcReorder);
		free(dcDepth);
	}
	__attribute__((fallthrough));	

	case INIT_PARTITION:
	MAB_WRAP("init partition") {
        unsigned j;

		//clear previous partitions
		if(_partition_cache){
			for(i=0; i<_max_res*_max_res; ++i)
				vec_destroy(&_partition_cache[i].partitions);
			free(_partition_cache);
			_partition_cache = NULL;
		}
        
        //create new partitions
        _max_res = pow(2, _max_depth);
		printf("partition res: (%ux%u)\n", _max_res, _max_res);
        unsigned long blkSize = _max_res*_max_res*sizeof(PartitionData);
        printf("Attempting to allocate %lu bytes for patition table\n", blkSize);
		_partition_cache = (PartitionData *)calloc(blkSize, sizeof(char));
        if(!_partition_cache){
            printf("Could not create partition cache of size %lu bytes!\n", blkSize);
            exit(1);
        }
			for(i=0; i<_max_res*_max_res; ++i)
				vec_init(&_partition_cache[i].partitions);
	
		GLfloat *vertsX = nattribs[0].floats;		
		GLfloat *vertsY = nattribs[1].floats;		

		GLfloat minX = nattribs[0].frange[0] - 1.0;
		GLfloat maxX = nattribs[0].frange[1] + 1.0;
		GLfloat minY = nattribs[1].frange[0] - 1.0;
		GLfloat maxY = nattribs[1].frange[1] + 1.0;

        printf("partitioning data...\n");
        long sz = edges->count;
        long tenth = sz/10;
        if(tenth == 0) ++tenth;
		for(i=0; i<sz; i+=2) {

            if(i%tenth < 2) printf("%d%% done...\n", (int)(100*(double)i/(double)sz));

			GLuint e0 = edges->uints[i+0];
			GLuint e1 = edges->uints[i+1];

			GLfloat x0 = vertsX[e0];
			GLfloat y0 = vertsY[e0];
			GLfloat x1 = vertsX[e1];
			GLfloat y1 = vertsY[e1];
				
			Vec_GLuint partitions = voxel_traversal(_max_res, _max_res, x0, y0, x1, y1, minX, minY, maxX, maxY);
            //printf("edge lies in %lu voxels\n", partitions.length);
			for(j=0; j<partitions.length; ++j){
                //printf("pushing edge (%d,%d) to partition %d\n", e0, e1, partitions.data[j]);
				assert(vec_push(&_partition_cache[partitions.data[j]].partitions, e0) != -1);
				assert(vec_push(&_partition_cache[partitions.data[j]].partitions, e1) != -1);
			}
			vec_destroy(&partitions);
		}
        printf("100%% done\n");

        //log_partition_cache(_partition_cache);
	}
	__attribute__((fallthrough));

	case INIT_BUFFERS:
	MAB_WRAP("init node buffers") {
		if (aNodeBuffers[0])
		for (i=0; i<ncount; ++i)
        glDeleteBuffers(1, &aNodeBuffers[i]);

		for (i=0; i<ncount; ++i)
		MAB_WRAP("init buffer") {
            glGenBuffers(1, &aNodeBuffers[i]);
            glBindBuffer(GL_ARRAY_BUFFER, aNodeBuffers[i]);
            MAB_WRAP("glBufferData") {
                mabLogMessage("size", "%lu", nattribs[i].size);
                glBufferData(GL_ARRAY_BUFFER, nattribs[i].size, nattribs[i].data, GL_STATIC_DRAW);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	case INIT_PROGRAM:
	MAB_WRAP("create program") {
		MAB_WRAP("create vertex shader") {
			if (vertexShader) {
                glDeleteShader(vertexShader);
            }
            vertexShader = glCreateShader(GL_VERTEX_SHADER);

            glShaderSource(vertexShader, 1, (const GLchar *const *)&_vertexShaderSource, NULL);

            glCompileShader(vertexShader);

			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &rc);
			if (!rc) {
				glGetShaderInfoLog(vertexShader, sizeof(log), NULL, log);
				printf("ERROR: Vertex Shader Compilation Failed\n%s\n", log);
				mabLogEnd("ERROR: Vertex Shader Compilation Failed");
				mabLogEnd("ERROR: Vertex Shader Compilation Failed");
				_error = ERROR_COMPILE_VERTEX_SHADER;
				goto done_with_request;
			}
		}

		MAB_WRAP("create fragment shader") {
			if (fragmentShader) {
                glDeleteShader(fragmentShader);
            }

            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, (const GLchar *const *)&_fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
	
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &rc);
			if (!rc) {
				glGetShaderInfoLog(fragmentShader, sizeof(log), NULL, log);
				printf("ERROR: Fragment Shader Compilation Failed\n%s\n", log);
				mabLogEnd("ERROR: Fragment Shader Compilation Failed");
				mabLogEnd("ERROR: Fragment Shader Compilation Failed");
				_error = ERROR_COMPILE_FRAGMENT_SHADER;
				goto done_with_request;
			}
		}

		MAB_WRAP("link program") {
			if (program) {
                glDeleteProgram(program);
            }

            program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
			
			glGetProgramiv(program, GL_LINK_STATUS, &rc);
			if(!rc){
				glGetProgramInfoLog(program, sizeof(log), NULL, log);
				printf("ERROR: Shader Program Linking Failed\n%s\n", log);
				mabLogEnd("ERROR: Shader Program Linking Failed");
				mabLogEnd("ERROR: Shader Program Linking Failed");
				_error = ERROR_LINK_PROGRAM;
				goto done_with_request;
			}
			
			glUseProgram(program);
		}
	}
	__attribute__((fallthrough));

	case INIT_ATTRIBUTES:
	MAB_WRAP("init node attributes") {
		for (i=0; i<ncount; ++i)
		MAB_WRAP("init attribute i=%d", i) {
			char temp[32];
			snprintf(temp, sizeof(temp), "aNode%lu", i + 1);

            aNodeLocations[i] = glGetAttribLocation(program, temp);
            glBindBuffer(GL_ARRAY_BUFFER, aNodeBuffers[i]);
            glVertexAttribPointer(aNodeLocations[i], 1, nattribs[i].type, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(aNodeLocations[i]);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	__attribute__((fallthrough));

	case INIT_UNIFORMS:
	MAB_WRAP("init uniforms") {
        uTranslateX = glGetUniformLocation(program, "uTranslateX");
        uTranslateY = glGetUniformLocation(program, "uTranslateY");
        uScale = glGetUniformLocation(program, "uScale");

		for (i=0; i<ncount; ++i) {
			char temp[32];
			snprintf(temp, sizeof(temp), "uNodeMin%lu", i + 1);
            uNodeMins[i] = glGetUniformLocation(program, temp);
		}

		for (i=0; i<ncount; ++i) {
			char temp[32];
			snprintf(temp, sizeof(temp), "uNodeMax%lu", i + 1);
            uNodeMaxs[i] = glGetUniformLocation(program, temp);
		}
	}
	__attribute__((fallthrough));

	case INIT_INDEX_BUFFER:
	MAB_WRAP("init index buffer") {
		/*
		unsigned i, j, k, curr;
		if (indexBuffer)
			glDeleteBuffers(1, &indexBuffer);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, edges->size, edges->data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		*/

		//partition cache index buffers
		for(i=0; i<_max_res*_max_res; ++i){
			if (_partition_cache[i].indexBuffer)
				glDeleteBuffers(1, &_partition_cache[i].indexBuffer);
		
            glGenBuffers(1, &_partition_cache[i].indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _partition_cache[i].indexBuffer);
            MAB_WRAP("glBufferData") {
                mabLogMessage("index", "%lu", (size_t)i);
                mabLogMessage("size", "%lu", _partition_cache[i].partitions.length*sizeof(GLuint));
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, _partition_cache[i].partitions.length*sizeof(GLuint), _partition_cache[i].partitions.data, GL_STATIC_DRAW);
            }
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	__attribute__((fallthrough));

	case RENDER:
	MAB_WRAP("render") {
        glUniform1f(uTranslateX, -1.0f * _x);
        glUniform1f(uTranslateY, -1.0f * _y);
        glUniform1f(uScale, pow(2.0f, _z));

		for (i=0; i<ncount; ++i) {
            glUniform1f(uNodeMins[i], nattribs[i].frange[0]);
            glUniform1f(uNodeMaxs[i], nattribs[i].frange[1]);
		}
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		/*
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_LINES, edges->count, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		*/
		
		//get current partition resolution by zoom level
		unsigned long res = (unsigned long)pow(2, _z);

		//figure out which partition we're rendering in
		unsigned long rx = (unsigned long)interpolate(0, res, 0, _max_res, _x);
		unsigned long ry = (unsigned long)interpolate(0, res, 0, _max_res, _y);
	    
        if(rx < _max_res && ry < _max_res){

		    int numTilesX = (int)ceil((float)_max_res/(float)res); 
            int numTilesY = numTilesX; //seperated just in case if in the future we want to have different x and y

            /*
            printf("res: (%lux%lu)/(%lux%lu)\n", res, res, _max_res, _max_res);
            printf("_x: %f, _y: %f\n", _x, _y);
            printf("rx: %lu, ry: %lu\n", rx, ry);
		    printf("------------- numTilesX: %d, numTilesY: %d -----------------\n", numTilesX, numTilesY);
            */

            MAB_WRAP("rendering tiles"){
                mabLogMessage("left", "%d", (int)rx);
                mabLogMessage("top", "%d", (int)ry);
                mabLogMessage("numTilesX", "%d", numTilesX);
                mabLogMessage("numTilesY", "%d", numTilesY);
                mabLogMessage("zoom", "%d", (int)_z);
		        for(i=0; i<numTilesX; ++i){
		    	    for(j=0; j<numTilesY; ++j){
		    		    unsigned long idx = (ry+j)*_max_res+(rx+i);
                        PartitionData *pd = &_partition_cache[idx];
                        pd->count++;

                        if(pd->partitions.length == 0) continue;

		    	    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _partition_cache[idx].indexBuffer);
                        MAB_WRAP("glDrawElements") {
                            printf("index: %lu\n", idx);
                            printf("size:  %lu\n", pd->partitions.length);
                            mabLogMessage("index", "%lu", idx);
                            mabLogMessage("size", "%lu", pd->partitions.length);
                            glDrawElements(GL_LINES, pd->partitions.length, GL_UNSIGNED_INT, 0);
                        }
		    	    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		     	    }
		        }
            }
        }

		glFinish();
	}
	__attribute__((fallthrough));

	case WAIT:
done_with_request:
	// give control of buffer
	pthread_barrier_wait(_barrier);

	// wait for them to be done
	pthread_barrier_wait(_barrier);
	mabLogEnd(NULL);

wait_for_request:
	// wait for request
	pthread_barrier_wait(_barrier);

	mabLogContinue(_info);
	mabLogAction("receive from request thread");

	where = RENDER;

	if (fabsf(x - _x) > 0.1) { x = _x; where = RENDER; }
	if (fabsf(y - _y) > 0.1) { y = _y; where = RENDER; }
	if (vertexShaderSource == NULL || strcmp(vertexShaderSource, _vertexShaderSource) != 0) {
		if (vertexShaderSource) free(vertexShaderSource);
		vertexShaderSource = strdup(_vertexShaderSource);
		where = INIT_PROGRAM;
	}
	if (fragmentShaderSource == NULL || strcmp(fragmentShaderSource, _fragmentShaderSource) != 0) {
		if (fragmentShaderSource) free(fragmentShaderSource);
		fragmentShaderSource = strdup(_fragmentShaderSource);
		where = INIT_PROGRAM;
	}
	if (max_depth != _max_depth) { max_depth = _max_depth; where = INIT_PARTITION; }
	if (dcIdent != _dcIdent) {
		dcIdent = _dcIdent;
		if (dcIndex) free(dcIndex);
		dcIndex = _dcIndex;
		if (dcMult) free(dcMult);
		dcMult = _dcMult;
		if (dcOffset) free(dcOffset);
		dcOffset = _dcOffset;
		dcMinMult = _dcMinMult;
		dcMaxMult = _dcMaxMult;
		where = INIT_DC;
	}
	if (dataset == NULL || strcmp(dataset, _dataset) != 0) {
		if (dataset) free(dataset);
		dataset = strdup(_dataset);
		where = INIT_GRAPH;
	}
	if (first) {
		first = GL_FALSE;
		where = INIT_OSMESA;
	}

	break;
	} /* switch */
} /* render */


size_t
tojpeg(void *rgba, int resolution, void **jpg, size_t *jpgsize) {
	size_t jpglen;

	mabLogMessage("resolution", "%d", resolution);
	mabLogMessage("rgba", "%p", rgba);

        *jpgsize = 1024;
        *jpg = malloc(*jpgsize);
        jpglen = 0;

        void mywriter(void *context, void *data, int size) {
		//mabLogMessage("size", "%d", size);
                if (jpglen + size > *jpgsize) {
			//mabLogMessage("realloc", "%lu bytes -> %lu bytes", *jpgsize, jpglen + size);
                        *jpgsize = jpglen + size;
                        *jpg = realloc(*jpg, *jpgsize);
                }
                memcpy(*jpg + jpglen, data, size);
                jpglen += size;
        }

	(void)stbi_write_jpg_to_func(mywriter, NULL, resolution, resolution, 4, rgba, 95);

	return jpglen;
}


size_t
topng(void *rgba, int resolution, void **png, size_t *pngsize) {
	size_t pnglen;

	mabLogMessage("resolution", "%d", resolution);
	mabLogMessage("rgba", "%p", rgba);

        *pngsize = 1024;
        *png = malloc(*pngsize);
        pnglen = 0;

        void mywriter(void *context, void *data, int size) {
                if (pnglen + size > *pngsize) {
                        *pngsize = pnglen + size;
                        *png = realloc(*png, *pngsize);
                }
                memcpy(*png + pnglen, data, size);
                pnglen += size;
        }

	(void)stbi_write_png_to_func(mywriter, NULL, resolution, resolution, 4, rgba, 0);

	return pnglen;
}


struct MHD_Response *
MAB_create_response_from_file(const char *filename) {
	FILE *f;
	size_t size, actual;
	char *data;

	f = fopen(filename, "rb");
	if (f == NULL) {
		fprintf(stderr, "could not open %s\n", filename);
		//exit(1);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	rewind(f);
	
	data = malloc(size);
	actual = fread(data, 1, size, f);
	if (actual != size) {
		fprintf(stderr, "didn't read all of %s\n", filename);
		//exit(1);
		free(data);
		fclose(f);
		return NULL;
	}
	fclose(f);
	
	return MHD_create_response_from_buffer(size, data, MHD_RESPMEM_MUST_FREE);
}

#define ANSWER(name) int name(void *cls, struct MHD_Connection *conn, const char *url, const char *method, const char *version, const char *data, size_t *size, void **con_cls)

ANSWER(Index) {
	struct MHD_Response *index_response;
	int rc;

	index_response = MAB_create_response_from_file("static/index.html");
	if(index_response == NULL){
		printf("Could not read file static/index.html\n");
		exit(1);
	}
	rc = MHD_queue_response(conn, MHD_HTTP_OK, index_response);
	MHD_destroy_response(index_response);
	return rc;
}

struct MHD_Response *error_response;
ANSWER(Error) {
	fprintf(stderr, "404: '%s' '%s'\n", method, url);
	return MHD_queue_response(conn, MHD_HTTP_NOT_FOUND, error_response);	
}

ANSWER(Static) {
	struct MHD_Response *response;
	int rc;

	const char *URL = url+1;

	response = MAB_create_response_from_file(URL);
	if(response == NULL){
		rc = Error(cls, conn, url, method, version, data, size, con_cls);
	}else{
		rc = MHD_queue_response(conn, MHD_HTTP_OK, response);
		MHD_destroy_response(response);
	}
	return rc;
}

ANSWER(Tile) { 
	int rc;
	struct MHD_Response *response;
	void *output;
	size_t outputlen;
	char *renderInfo;

	const char *info = MHD_lookup_connection_value(conn, MHD_HEADER_KIND, "X-MAB-Log-Info");
	if (info != NULL) {
		fprintf(stderr, "info = '%s'\n", info);
		mabLogContinue(info);
	}

	MAB_WRAP("answer tile request") {
    mabLogMessage("rxsize", "%zu", strlen(url));
	float x, y, z;
    int max_depth;
	char *dataset, *options;
	if (5 != (rc = sscanf(url, "/tile/%m[^/]/%f/%f/%f/%ms", &dataset, &z, &x, &y, &options))) {
		fprintf(stderr, "rc: %d\n", rc);
		fprintf(stderr, "options: %s\n", options);
		rc = MHD_queue_response(conn, MHD_HTTP_NOT_FOUND, error_response);	
		goto end;
	}
	mabLogMessage("x", "%f", x);
	mabLogMessage("y", "%f", y);
	mabLogMessage("z", "%f", z);
	mabLogMessage("dataset", "%s", dataset);
	mabLogMessage("options", "%s", options);

	char *s;

	char *opt_vert = NULL;
	char *opt_frag = NULL;
	GLuint opt_dcIdent = 0;
	GLuint *opt_dcIndex = NULL;
	GLfloat *opt_dcMult = NULL;
	GLfloat *opt_dcOffset = NULL;
	GLfloat opt_dcMinMult = 0.0;
	GLfloat opt_dcMaxMult = 0.0;

	//fprintf(stderr, "options: '''\n%s\n'''\n", options);

	char *it = options + 1;
	for (;;) {
		char *key = it;

		it = strchrnul(it, ',');
		if (!*it) break;
		*it++ = '\0';

		void *value = it;
		
		it = strchrnul(it, ',');
		int done = *it == '\0';
		*it++ = '\0';

		if (strncmp(value, "base64:", strlen("base64:")) == 0) {
			char *data = value + strlen("base64:");
			size_t len = strlen(data);

			size_t outlen = len + 128;
			char *out = malloc(outlen + 1);
			if (base64decode(data, len, out, &outlen) != 0) {
				fprintf(stderr, "ERROR: base64decode failed\n");
				return MHD_NO;
			}

			out[outlen] = '\0';
			value = out;
		}

		if (strcmp(key, "vert") == 0) {
			opt_vert = value;
		} else if (strcmp(key, "frag") == 0) {
			opt_frag = value;
		} else if (strcmp(key, "pDepth") == 0){
            max_depth = atoi(value);
        } else if (strcmp(key, "dcIdent") == 0) {
			opt_dcIdent = atoi(value);
		} else if (strcmp(key, "dcIndex") == 0) {
			opt_dcIndex = value;
		} else if (strcmp(key, "dcMult") == 0) {
			opt_dcMult = value;
		} else if (strcmp(key, "dcOffset") == 0) {
			opt_dcOffset = value;
		} else if (strcmp(key, "dcMinMult") == 0) {
			opt_dcMinMult = atof(value);
		} else if (strcmp(key, "dcMaxMult") == 0) {
			opt_dcMaxMult = atof(value);
		} else {
			fprintf(stderr, "WARNING: unhandled option '%s' = '%s'\n", key, (char *)value);
		}

		if (done) break;
	}

	mabLogMessage("opt_vert length", "%d", strlen(opt_vert));
	mabLogMessage("opt_frag length", "%d", strlen(opt_frag));
	mabLogMessage("opt_dcIdent", "%u", opt_dcIdent);

	MAB_WRAP("send to render thread") {
		MAB_WRAP("lock")
		pthread_mutex_lock(_lock);
        
        _max_depth = max_depth;
		_x = x;
		_y = y;
		_z = z;
		_dataset = dataset;
		_vertexShaderSource = opt_vert;
		_fragmentShaderSource = opt_frag;
		_dcIdent = opt_dcIdent;
		_dcIndex = opt_dcIndex;
		_dcMult = opt_dcMult;
		_dcOffset = opt_dcOffset;
		_dcMinMult = opt_dcMinMult;
		_dcMaxMult = opt_dcMaxMult;
		_error = ERROR_NONE;

		mabLogForward(&renderInfo);
		_info = renderInfo;
		
		// tell render thread to run
		pthread_barrier_wait(_barrier);

		// wait for render to be finished
		pthread_barrier_wait(_barrier);

		if (_error != ERROR_NONE) {
			char *message;
			message = errorMessages[_error];

            mabLogMessage("error", "%s", message);

			response = MHD_create_response_from_buffer(strlen(message), message, MHD_RESPMEM_PERSISTENT);
			rc = MHD_queue_response(conn, MHD_HTTP_NOT_ACCEPTABLE, response);
			MHD_destroy_response(response);
			
			// tell render thread we're done
			pthread_barrier_wait(_barrier);

			MAB_WRAP("unlock")
			pthread_mutex_unlock(_lock);

			goto end;

		} else {
			MAB_WRAP("png") {
				output = NULL;
				outputlen = 0;
				topng(_image, _resolution, &output, &outputlen);
			}

			// tell render thread we're done
			pthread_barrier_wait(_barrier);

			MAB_WRAP("unlock")
			pthread_mutex_unlock(_lock);
		}
	}

    mabLogMessage("txsize", "%zu", outputlen);
	
	response = MHD_create_response_from_buffer(outputlen, output, MHD_RESPMEM_MUST_FREE);
	
	const char *origin = MHD_lookup_connection_value(conn, MHD_HEADER_KIND, "origin");
	if (origin != NULL) {
		MHD_add_response_header(response, "Access-Control-Allow-Origin", origin);
	}
	MHD_add_response_header(response, "Content-Type", "image/png");
	MHD_add_response_header(response, "Connection", "close");
	
	rc = MHD_queue_response(conn, MHD_HTTP_OK, response);
	MHD_destroy_response(response);
	
end: ;
	}
	return rc;
}

ANSWER(Log) {
	struct MHD_Response *response;
	int rc;

	char *message = strdup(url + strlen("/log/"));
	size_t len = MHD_http_unescape(message);

	mabLogDirectly(message);

	response = MHD_create_response_from_buffer(len, message, MHD_RESPMEM_MUST_COPY);
	rc = MHD_queue_response(conn, MHD_HTTP_OK, response);
	MHD_destroy_response(response);
	return rc;
}

struct {
	const char *method;
	const char *url;
	int exact;
	MHD_AccessHandlerCallback cb;
} routes[] = {
	{ NULL, NULL, 0, Error },
	{ "GET", "/", 1, Index },
	{ "GET", "/static/", 0, Static },
	{ "GET", "/tile/", 0, Tile },
	{ "POST", "/log/", 0, Log },
};

ANSWER(answer) {
	size_t i;

	for (i=1; i<sizeof(routes)/sizeof(*routes); ++i) {
		if (strcmp(method, routes[i].method) != 0) continue;
		if (routes[i].exact && strcmp(url, routes[i].url) != 0) continue;
		if (!routes[i].exact && strncmp(url, routes[i].url, strlen(routes[i].url)) != 0) continue;
		
		return routes[i].cb(cls, conn, url, method, version, data, size, con_cls);
	}
	
	return routes[0].cb(cls, conn, url, method, version, data, size, con_cls);
}

static void
initialize(void) {
	error_response = MAB_create_response_from_file("static/error.html");
	if(error_response == NULL){
		printf("Could not read file static/error.html");
		exit(1);
	}
}

int
main(int argc, char **argv) {
	int opt_port, opt_service;
	char *s, *opt_bind;
	struct MHD_Daemon *daemon;
	struct sockaddr_in addr;
	pthread_t tid;

	for (int i=0; i<32; ++i) {
		char temp[32];
		snprintf(temp, sizeof(temp), "logs/%d.log", i);

		if (!mabLogToFile(temp, "wx")) {
			goto got_log;
			break;
		}
	}

	fprintf(stderr, "could not open log\n");
	return 1;

got_log:
	_lock = malloc(sizeof(*_lock));
	pthread_mutex_init(_lock, NULL);

	_barrier = malloc(sizeof(*_barrier));
	pthread_barrier_init(_barrier, NULL, 2);

	pthread_create(&tid, NULL, render, NULL);

	pthread_barrier_wait(_barrier);

	MAB_WRAP("server main loop") {

	opt_port = (s = getenv("FG_PORT")) ? atoi(s) : 8889;
	opt_bind = (s = getenv("FG_BIND")) ? s : "0.0.0.0";
	opt_service = (s = getenv("FG_SERVICE")) ? atoi(s) : 0;

	mabLogMessage("port", "%d", opt_port);
	mabLogMessage("bind", "%s", opt_bind);
	
	fprintf(stderr, "Listening on %s:%d\n", opt_bind, opt_port);
	
	mabLogAction("initialize");
	initialize();
	mabLogEnd(NULL);

	memset(&addr, sizeof(addr), 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(opt_port);
	inet_aton(opt_bind, &addr.sin_addr);
	
	daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, 0, NULL, NULL, &answer, NULL, MHD_OPTION_SOCK_ADDR, &addr, MHD_OPTION_END);
	if (daemon == NULL) {
		fprintf(stderr, "could not start daemon\n");
		return 1;
	}
	
	if (opt_service) {
		while (sleep(60 * 60) == 0)
			;
	} else {
		fprintf(stderr, "press enter to stop\n");
		getchar();
	}
	fprintf(stderr, "stopping...\n");
	
	MHD_stop_daemon(daemon);
	}
	return 0;
}
