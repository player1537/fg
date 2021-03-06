/*

    EGL loader generated by glad 0.1.34 on Mon Apr  5 20:50:26 2021.

    Language/Generator: C/C++
    Specification: egl
    APIs: egl=1.5
    Profile: -
    Extensions:
        EGL_EXT_buffer_age,
        EGL_EXT_client_sync,
        EGL_EXT_create_context_robustness,
        EGL_EXT_image_dma_buf_import,
        EGL_EXT_image_dma_buf_import_modifiers,
        EGL_EXT_output_base,
        EGL_EXT_output_drm,
        EGL_EXT_protected_content,
        EGL_EXT_stream_consumer_egloutput,
        EGL_EXT_sync_reuse,
        EGL_IMG_context_priority,
        EGL_KHR_config_attribs,
        EGL_KHR_context_flush_control,
        EGL_KHR_create_context,
        EGL_KHR_create_context_no_error,
        EGL_KHR_fence_sync,
        EGL_KHR_get_all_proc_addresses,
        EGL_KHR_gl_colorspace,
        EGL_KHR_gl_renderbuffer_image,
        EGL_KHR_gl_texture_2D_image,
        EGL_KHR_gl_texture_3D_image,
        EGL_KHR_gl_texture_cubemap_image,
        EGL_KHR_image,
        EGL_KHR_image_base,
        EGL_KHR_no_config_context,
        EGL_KHR_partial_update,
        EGL_KHR_reusable_sync,
        EGL_KHR_stream,
        EGL_KHR_stream_attrib,
        EGL_KHR_stream_consumer_gltexture,
        EGL_KHR_stream_cross_process_fd,
        EGL_KHR_stream_fifo,
        EGL_KHR_stream_producer_eglsurface,
        EGL_KHR_surfaceless_context,
        EGL_KHR_swap_buffers_with_damage,
        EGL_KHR_wait_sync,
        EGL_MESA_image_dma_buf_export,
        EGL_NV_quadruple_buffer,
        EGL_NV_stream_consumer_eglimage,
        EGL_NV_stream_consumer_gltexture_yuv,
        EGL_NV_stream_cross_display,
        EGL_NV_stream_cross_object,
        EGL_NV_stream_cross_process,
        EGL_NV_stream_cross_system,
        EGL_NV_stream_dma,
        EGL_NV_stream_fifo_next,
        EGL_NV_stream_fifo_synchronous,
        EGL_NV_stream_flush,
        EGL_NV_stream_metadata,
        EGL_NV_stream_origin,
        EGL_NV_stream_remote,
        EGL_NV_stream_reset,
        EGL_NV_stream_socket,
        EGL_NV_stream_socket_inet,
        EGL_NV_stream_socket_unix,
        EGL_NV_stream_sync,
        EGL_NV_system_time,
        EGL_NV_triple_buffer
    Loader: True
    Local files: True
    Omit khrplatform: False
    Reproducible: False

    Commandline:
        --api="egl=1.5" --generator="c" --spec="egl" --local-files --extensions="EGL_EXT_buffer_age,EGL_EXT_client_sync,EGL_EXT_create_context_robustness,EGL_EXT_image_dma_buf_import,EGL_EXT_image_dma_buf_import_modifiers,EGL_EXT_output_base,EGL_EXT_output_drm,EGL_EXT_protected_content,EGL_EXT_stream_consumer_egloutput,EGL_EXT_sync_reuse,EGL_IMG_context_priority,EGL_KHR_config_attribs,EGL_KHR_context_flush_control,EGL_KHR_create_context,EGL_KHR_create_context_no_error,EGL_KHR_fence_sync,EGL_KHR_get_all_proc_addresses,EGL_KHR_gl_colorspace,EGL_KHR_gl_renderbuffer_image,EGL_KHR_gl_texture_2D_image,EGL_KHR_gl_texture_3D_image,EGL_KHR_gl_texture_cubemap_image,EGL_KHR_image,EGL_KHR_image_base,EGL_KHR_no_config_context,EGL_KHR_partial_update,EGL_KHR_reusable_sync,EGL_KHR_stream,EGL_KHR_stream_attrib,EGL_KHR_stream_consumer_gltexture,EGL_KHR_stream_cross_process_fd,EGL_KHR_stream_fifo,EGL_KHR_stream_producer_eglsurface,EGL_KHR_surfaceless_context,EGL_KHR_swap_buffers_with_damage,EGL_KHR_wait_sync,EGL_MESA_image_dma_buf_export,EGL_NV_quadruple_buffer,EGL_NV_stream_consumer_eglimage,EGL_NV_stream_consumer_gltexture_yuv,EGL_NV_stream_cross_display,EGL_NV_stream_cross_object,EGL_NV_stream_cross_process,EGL_NV_stream_cross_system,EGL_NV_stream_dma,EGL_NV_stream_fifo_next,EGL_NV_stream_fifo_synchronous,EGL_NV_stream_flush,EGL_NV_stream_metadata,EGL_NV_stream_origin,EGL_NV_stream_remote,EGL_NV_stream_reset,EGL_NV_stream_socket,EGL_NV_stream_socket_inet,EGL_NV_stream_socket_unix,EGL_NV_stream_sync,EGL_NV_system_time,EGL_NV_triple_buffer"
    Online:
        Too many extensions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glad/glad_egl.h"

int gladLoadEGL(void) {
    return gladLoadEGLLoader((GLADloadproc)eglGetProcAddress);
}

PFNEGLCLIENTSIGNALSYNCEXTPROC glad_eglClientSignalSyncEXT = NULL;
PFNEGLQUERYDMABUFFORMATSEXTPROC glad_eglQueryDmaBufFormatsEXT = NULL;
PFNEGLQUERYDMABUFMODIFIERSEXTPROC glad_eglQueryDmaBufModifiersEXT = NULL;
PFNEGLGETOUTPUTLAYERSEXTPROC glad_eglGetOutputLayersEXT = NULL;
PFNEGLGETOUTPUTPORTSEXTPROC glad_eglGetOutputPortsEXT = NULL;
PFNEGLOUTPUTLAYERATTRIBEXTPROC glad_eglOutputLayerAttribEXT = NULL;
PFNEGLQUERYOUTPUTLAYERATTRIBEXTPROC glad_eglQueryOutputLayerAttribEXT = NULL;
PFNEGLQUERYOUTPUTLAYERSTRINGEXTPROC glad_eglQueryOutputLayerStringEXT = NULL;
PFNEGLOUTPUTPORTATTRIBEXTPROC glad_eglOutputPortAttribEXT = NULL;
PFNEGLQUERYOUTPUTPORTATTRIBEXTPROC glad_eglQueryOutputPortAttribEXT = NULL;
PFNEGLQUERYOUTPUTPORTSTRINGEXTPROC glad_eglQueryOutputPortStringEXT = NULL;
PFNEGLSTREAMCONSUMEROUTPUTEXTPROC glad_eglStreamConsumerOutputEXT = NULL;
PFNEGLUNSIGNALSYNCEXTPROC glad_eglUnsignalSyncEXT = NULL;
PFNEGLCREATESYNCKHRPROC glad_eglCreateSyncKHR = NULL;
PFNEGLDESTROYSYNCKHRPROC glad_eglDestroySyncKHR = NULL;
PFNEGLCLIENTWAITSYNCKHRPROC glad_eglClientWaitSyncKHR = NULL;
PFNEGLGETSYNCATTRIBKHRPROC glad_eglGetSyncAttribKHR = NULL;
PFNEGLCREATEIMAGEKHRPROC glad_eglCreateImageKHR = NULL;
PFNEGLDESTROYIMAGEKHRPROC glad_eglDestroyImageKHR = NULL;
PFNEGLSETDAMAGEREGIONKHRPROC glad_eglSetDamageRegionKHR = NULL;
PFNEGLSIGNALSYNCKHRPROC glad_eglSignalSyncKHR = NULL;
PFNEGLCREATESTREAMKHRPROC glad_eglCreateStreamKHR = NULL;
PFNEGLDESTROYSTREAMKHRPROC glad_eglDestroyStreamKHR = NULL;
PFNEGLSTREAMATTRIBKHRPROC glad_eglStreamAttribKHR = NULL;
PFNEGLQUERYSTREAMKHRPROC glad_eglQueryStreamKHR = NULL;
PFNEGLQUERYSTREAMU64KHRPROC glad_eglQueryStreamu64KHR = NULL;
PFNEGLCREATESTREAMATTRIBKHRPROC glad_eglCreateStreamAttribKHR = NULL;
PFNEGLSETSTREAMATTRIBKHRPROC glad_eglSetStreamAttribKHR = NULL;
PFNEGLQUERYSTREAMATTRIBKHRPROC glad_eglQueryStreamAttribKHR = NULL;
PFNEGLSTREAMCONSUMERACQUIREATTRIBKHRPROC glad_eglStreamConsumerAcquireAttribKHR = NULL;
PFNEGLSTREAMCONSUMERRELEASEATTRIBKHRPROC glad_eglStreamConsumerReleaseAttribKHR = NULL;
PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALKHRPROC glad_eglStreamConsumerGLTextureExternalKHR = NULL;
PFNEGLSTREAMCONSUMERACQUIREKHRPROC glad_eglStreamConsumerAcquireKHR = NULL;
PFNEGLSTREAMCONSUMERRELEASEKHRPROC glad_eglStreamConsumerReleaseKHR = NULL;
PFNEGLGETSTREAMFILEDESCRIPTORKHRPROC glad_eglGetStreamFileDescriptorKHR = NULL;
PFNEGLCREATESTREAMFROMFILEDESCRIPTORKHRPROC glad_eglCreateStreamFromFileDescriptorKHR = NULL;
PFNEGLQUERYSTREAMTIMEKHRPROC glad_eglQueryStreamTimeKHR = NULL;
PFNEGLCREATESTREAMPRODUCERSURFACEKHRPROC glad_eglCreateStreamProducerSurfaceKHR = NULL;
PFNEGLSWAPBUFFERSWITHDAMAGEKHRPROC glad_eglSwapBuffersWithDamageKHR = NULL;
PFNEGLWAITSYNCKHRPROC glad_eglWaitSyncKHR = NULL;
PFNEGLEXPORTDMABUFIMAGEQUERYMESAPROC glad_eglExportDMABUFImageQueryMESA = NULL;
PFNEGLEXPORTDMABUFIMAGEMESAPROC glad_eglExportDMABUFImageMESA = NULL;
PFNEGLSTREAMIMAGECONSUMERCONNECTNVPROC glad_eglStreamImageConsumerConnectNV = NULL;
PFNEGLQUERYSTREAMCONSUMEREVENTNVPROC glad_eglQueryStreamConsumerEventNV = NULL;
PFNEGLSTREAMACQUIREIMAGENVPROC glad_eglStreamAcquireImageNV = NULL;
PFNEGLSTREAMRELEASEIMAGENVPROC glad_eglStreamReleaseImageNV = NULL;
PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALATTRIBSNVPROC glad_eglStreamConsumerGLTextureExternalAttribsNV = NULL;
PFNEGLSTREAMFLUSHNVPROC glad_eglStreamFlushNV = NULL;
PFNEGLQUERYDISPLAYATTRIBNVPROC glad_eglQueryDisplayAttribNV = NULL;
PFNEGLSETSTREAMMETADATANVPROC glad_eglSetStreamMetadataNV = NULL;
PFNEGLQUERYSTREAMMETADATANVPROC glad_eglQueryStreamMetadataNV = NULL;
PFNEGLRESETSTREAMNVPROC glad_eglResetStreamNV = NULL;
PFNEGLCREATESTREAMSYNCNVPROC glad_eglCreateStreamSyncNV = NULL;
PFNEGLGETSYSTEMTIMEFREQUENCYNVPROC glad_eglGetSystemTimeFrequencyNV = NULL;
PFNEGLGETSYSTEMTIMENVPROC glad_eglGetSystemTimeNV = NULL;
static void load_EGL_EXT_client_sync(GLADloadproc load) {
	glad_eglClientSignalSyncEXT = (PFNEGLCLIENTSIGNALSYNCEXTPROC)load("eglClientSignalSyncEXT");
}
static void load_EGL_EXT_image_dma_buf_import_modifiers(GLADloadproc load) {
	glad_eglQueryDmaBufFormatsEXT = (PFNEGLQUERYDMABUFFORMATSEXTPROC)load("eglQueryDmaBufFormatsEXT");
	glad_eglQueryDmaBufModifiersEXT = (PFNEGLQUERYDMABUFMODIFIERSEXTPROC)load("eglQueryDmaBufModifiersEXT");
}
static void load_EGL_EXT_output_base(GLADloadproc load) {
	glad_eglGetOutputLayersEXT = (PFNEGLGETOUTPUTLAYERSEXTPROC)load("eglGetOutputLayersEXT");
	glad_eglGetOutputPortsEXT = (PFNEGLGETOUTPUTPORTSEXTPROC)load("eglGetOutputPortsEXT");
	glad_eglOutputLayerAttribEXT = (PFNEGLOUTPUTLAYERATTRIBEXTPROC)load("eglOutputLayerAttribEXT");
	glad_eglQueryOutputLayerAttribEXT = (PFNEGLQUERYOUTPUTLAYERATTRIBEXTPROC)load("eglQueryOutputLayerAttribEXT");
	glad_eglQueryOutputLayerStringEXT = (PFNEGLQUERYOUTPUTLAYERSTRINGEXTPROC)load("eglQueryOutputLayerStringEXT");
	glad_eglOutputPortAttribEXT = (PFNEGLOUTPUTPORTATTRIBEXTPROC)load("eglOutputPortAttribEXT");
	glad_eglQueryOutputPortAttribEXT = (PFNEGLQUERYOUTPUTPORTATTRIBEXTPROC)load("eglQueryOutputPortAttribEXT");
	glad_eglQueryOutputPortStringEXT = (PFNEGLQUERYOUTPUTPORTSTRINGEXTPROC)load("eglQueryOutputPortStringEXT");
}
static void load_EGL_EXT_stream_consumer_egloutput(GLADloadproc load) {
	glad_eglStreamConsumerOutputEXT = (PFNEGLSTREAMCONSUMEROUTPUTEXTPROC)load("eglStreamConsumerOutputEXT");
}
static void load_EGL_EXT_sync_reuse(GLADloadproc load) {
	glad_eglUnsignalSyncEXT = (PFNEGLUNSIGNALSYNCEXTPROC)load("eglUnsignalSyncEXT");
}
static void load_EGL_KHR_fence_sync(GLADloadproc load) {
	glad_eglCreateSyncKHR = (PFNEGLCREATESYNCKHRPROC)load("eglCreateSyncKHR");
	glad_eglDestroySyncKHR = (PFNEGLDESTROYSYNCKHRPROC)load("eglDestroySyncKHR");
	glad_eglClientWaitSyncKHR = (PFNEGLCLIENTWAITSYNCKHRPROC)load("eglClientWaitSyncKHR");
	glad_eglGetSyncAttribKHR = (PFNEGLGETSYNCATTRIBKHRPROC)load("eglGetSyncAttribKHR");
}
static void load_EGL_KHR_image(GLADloadproc load) {
	glad_eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)load("eglCreateImageKHR");
	glad_eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)load("eglDestroyImageKHR");
}
static void load_EGL_KHR_image_base(GLADloadproc load) {
	glad_eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)load("eglCreateImageKHR");
	glad_eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)load("eglDestroyImageKHR");
}
static void load_EGL_KHR_partial_update(GLADloadproc load) {
	glad_eglSetDamageRegionKHR = (PFNEGLSETDAMAGEREGIONKHRPROC)load("eglSetDamageRegionKHR");
}
static void load_EGL_KHR_reusable_sync(GLADloadproc load) {
	glad_eglCreateSyncKHR = (PFNEGLCREATESYNCKHRPROC)load("eglCreateSyncKHR");
	glad_eglDestroySyncKHR = (PFNEGLDESTROYSYNCKHRPROC)load("eglDestroySyncKHR");
	glad_eglClientWaitSyncKHR = (PFNEGLCLIENTWAITSYNCKHRPROC)load("eglClientWaitSyncKHR");
	glad_eglSignalSyncKHR = (PFNEGLSIGNALSYNCKHRPROC)load("eglSignalSyncKHR");
	glad_eglGetSyncAttribKHR = (PFNEGLGETSYNCATTRIBKHRPROC)load("eglGetSyncAttribKHR");
}
static void load_EGL_KHR_stream(GLADloadproc load) {
	glad_eglCreateStreamKHR = (PFNEGLCREATESTREAMKHRPROC)load("eglCreateStreamKHR");
	glad_eglDestroyStreamKHR = (PFNEGLDESTROYSTREAMKHRPROC)load("eglDestroyStreamKHR");
	glad_eglStreamAttribKHR = (PFNEGLSTREAMATTRIBKHRPROC)load("eglStreamAttribKHR");
	glad_eglQueryStreamKHR = (PFNEGLQUERYSTREAMKHRPROC)load("eglQueryStreamKHR");
	glad_eglQueryStreamu64KHR = (PFNEGLQUERYSTREAMU64KHRPROC)load("eglQueryStreamu64KHR");
}
static void load_EGL_KHR_stream_attrib(GLADloadproc load) {
	glad_eglCreateStreamAttribKHR = (PFNEGLCREATESTREAMATTRIBKHRPROC)load("eglCreateStreamAttribKHR");
	glad_eglSetStreamAttribKHR = (PFNEGLSETSTREAMATTRIBKHRPROC)load("eglSetStreamAttribKHR");
	glad_eglQueryStreamAttribKHR = (PFNEGLQUERYSTREAMATTRIBKHRPROC)load("eglQueryStreamAttribKHR");
	glad_eglStreamConsumerAcquireAttribKHR = (PFNEGLSTREAMCONSUMERACQUIREATTRIBKHRPROC)load("eglStreamConsumerAcquireAttribKHR");
	glad_eglStreamConsumerReleaseAttribKHR = (PFNEGLSTREAMCONSUMERRELEASEATTRIBKHRPROC)load("eglStreamConsumerReleaseAttribKHR");
}
static void load_EGL_KHR_stream_consumer_gltexture(GLADloadproc load) {
	glad_eglStreamConsumerGLTextureExternalKHR = (PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALKHRPROC)load("eglStreamConsumerGLTextureExternalKHR");
	glad_eglStreamConsumerAcquireKHR = (PFNEGLSTREAMCONSUMERACQUIREKHRPROC)load("eglStreamConsumerAcquireKHR");
	glad_eglStreamConsumerReleaseKHR = (PFNEGLSTREAMCONSUMERRELEASEKHRPROC)load("eglStreamConsumerReleaseKHR");
}
static void load_EGL_KHR_stream_cross_process_fd(GLADloadproc load) {
	glad_eglGetStreamFileDescriptorKHR = (PFNEGLGETSTREAMFILEDESCRIPTORKHRPROC)load("eglGetStreamFileDescriptorKHR");
	glad_eglCreateStreamFromFileDescriptorKHR = (PFNEGLCREATESTREAMFROMFILEDESCRIPTORKHRPROC)load("eglCreateStreamFromFileDescriptorKHR");
}
static void load_EGL_KHR_stream_fifo(GLADloadproc load) {
	glad_eglQueryStreamTimeKHR = (PFNEGLQUERYSTREAMTIMEKHRPROC)load("eglQueryStreamTimeKHR");
}
static void load_EGL_KHR_stream_producer_eglsurface(GLADloadproc load) {
	glad_eglCreateStreamProducerSurfaceKHR = (PFNEGLCREATESTREAMPRODUCERSURFACEKHRPROC)load("eglCreateStreamProducerSurfaceKHR");
}
static void load_EGL_KHR_swap_buffers_with_damage(GLADloadproc load) {
	glad_eglSwapBuffersWithDamageKHR = (PFNEGLSWAPBUFFERSWITHDAMAGEKHRPROC)load("eglSwapBuffersWithDamageKHR");
}
static void load_EGL_KHR_wait_sync(GLADloadproc load) {
	glad_eglWaitSyncKHR = (PFNEGLWAITSYNCKHRPROC)load("eglWaitSyncKHR");
}
static void load_EGL_MESA_image_dma_buf_export(GLADloadproc load) {
	glad_eglExportDMABUFImageQueryMESA = (PFNEGLEXPORTDMABUFIMAGEQUERYMESAPROC)load("eglExportDMABUFImageQueryMESA");
	glad_eglExportDMABUFImageMESA = (PFNEGLEXPORTDMABUFIMAGEMESAPROC)load("eglExportDMABUFImageMESA");
}
static void load_EGL_NV_stream_consumer_eglimage(GLADloadproc load) {
	glad_eglStreamImageConsumerConnectNV = (PFNEGLSTREAMIMAGECONSUMERCONNECTNVPROC)load("eglStreamImageConsumerConnectNV");
	glad_eglQueryStreamConsumerEventNV = (PFNEGLQUERYSTREAMCONSUMEREVENTNVPROC)load("eglQueryStreamConsumerEventNV");
	glad_eglStreamAcquireImageNV = (PFNEGLSTREAMACQUIREIMAGENVPROC)load("eglStreamAcquireImageNV");
	glad_eglStreamReleaseImageNV = (PFNEGLSTREAMRELEASEIMAGENVPROC)load("eglStreamReleaseImageNV");
}
static void load_EGL_NV_stream_consumer_gltexture_yuv(GLADloadproc load) {
	glad_eglStreamConsumerGLTextureExternalAttribsNV = (PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALATTRIBSNVPROC)load("eglStreamConsumerGLTextureExternalAttribsNV");
}
static void load_EGL_NV_stream_flush(GLADloadproc load) {
	glad_eglStreamFlushNV = (PFNEGLSTREAMFLUSHNVPROC)load("eglStreamFlushNV");
}
static void load_EGL_NV_stream_metadata(GLADloadproc load) {
	glad_eglQueryDisplayAttribNV = (PFNEGLQUERYDISPLAYATTRIBNVPROC)load("eglQueryDisplayAttribNV");
	glad_eglSetStreamMetadataNV = (PFNEGLSETSTREAMMETADATANVPROC)load("eglSetStreamMetadataNV");
	glad_eglQueryStreamMetadataNV = (PFNEGLQUERYSTREAMMETADATANVPROC)load("eglQueryStreamMetadataNV");
}
static void load_EGL_NV_stream_reset(GLADloadproc load) {
	glad_eglResetStreamNV = (PFNEGLRESETSTREAMNVPROC)load("eglResetStreamNV");
}
static void load_EGL_NV_stream_sync(GLADloadproc load) {
	glad_eglCreateStreamSyncNV = (PFNEGLCREATESTREAMSYNCNVPROC)load("eglCreateStreamSyncNV");
}
static void load_EGL_NV_system_time(GLADloadproc load) {
	glad_eglGetSystemTimeFrequencyNV = (PFNEGLGETSYSTEMTIMEFREQUENCYNVPROC)load("eglGetSystemTimeFrequencyNV");
	glad_eglGetSystemTimeNV = (PFNEGLGETSYSTEMTIMENVPROC)load("eglGetSystemTimeNV");
}
static int find_extensionsEGL(void) {
	return 1;
}

static void find_coreEGL(void) {
	
}

int gladLoadEGLLoader(GLADloadproc load) {
	(void) load;
	find_coreEGL();

	if (!find_extensionsEGL()) return 0;
	load_EGL_EXT_client_sync(load);
	load_EGL_EXT_image_dma_buf_import_modifiers(load);
	load_EGL_EXT_output_base(load);
	load_EGL_EXT_stream_consumer_egloutput(load);
	load_EGL_EXT_sync_reuse(load);
	load_EGL_KHR_fence_sync(load);
	load_EGL_KHR_image(load);
	load_EGL_KHR_image_base(load);
	load_EGL_KHR_partial_update(load);
	load_EGL_KHR_reusable_sync(load);
	load_EGL_KHR_stream(load);
	load_EGL_KHR_stream_attrib(load);
	load_EGL_KHR_stream_consumer_gltexture(load);
	load_EGL_KHR_stream_cross_process_fd(load);
	load_EGL_KHR_stream_fifo(load);
	load_EGL_KHR_stream_producer_eglsurface(load);
	load_EGL_KHR_swap_buffers_with_damage(load);
	load_EGL_KHR_wait_sync(load);
	load_EGL_MESA_image_dma_buf_export(load);
	load_EGL_NV_stream_consumer_eglimage(load);
	load_EGL_NV_stream_consumer_gltexture_yuv(load);
	load_EGL_NV_stream_flush(load);
	load_EGL_NV_stream_metadata(load);
	load_EGL_NV_stream_reset(load);
	load_EGL_NV_stream_sync(load);
	load_EGL_NV_system_time(load);
	return 1;
}

