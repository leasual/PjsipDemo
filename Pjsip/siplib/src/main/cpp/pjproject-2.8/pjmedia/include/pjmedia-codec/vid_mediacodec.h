/* $Id: vid_mediacodec.h 5603 2019-08-08 16:23:56Z ming $ */

#ifndef __PJMEDIA_CODEC_VID_MEDIACODEC_H__
#define __PJMEDIA_CODEC_VID_MEDIACODEC_H__

#include <pjmedia-codec/types.h>
#include <pjmedia/vid_codec.h>

/**
 * @file pjmedia-codec/vid_mediacodec.h
 * @brief Video Android Media codec
 */


PJ_BEGIN_DECL

/**
 * @defgroup PJMEDIA_CODEC_VID_MEDIACODEC Video Toolbox Codec
 * @ingroup PJMEDIA_CODEC_VID_CODECS
 * @{
 *
 * Video MediaCodec wrapper for Android.
 */

/**
 * Initialize and register Video MediaCodec factory.
 *
 * @param mgr	    The video codec manager instance where this codec will
 * 		    be registered to. Specify NULL to use default instance
 * 		    (in that case, an instance of video codec manager must
 * 		    have been created beforehand).
 * @param pf	    Pool factory.
 *
 * @return	    PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_codec_vid_mediacodec_init(pjmedia_vid_codec_mgr *mgr,
                                                     pj_pool_factory *pf);

/**
 * Unregister Video MediaCodec factory from the video codec manager
 * and deinitialize the codec library.
 *
 * @return	    PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_codec_vid_mediacodec_deinit(void);


/**
 * @}  PJMEDIA_CODEC_VID_MEDIACODEC
 */


PJ_END_DECL

#endif	/* __PJMEDIA_CODEC_VID_MEDIACODEC_H__ */
