#include <pjmedia-codec/vid_mediacodec.h>
#include <pjmedia-codec/h264_packetizer.h>
#include <pjmedia/vid_codec_util.h>
#include <pjmedia/errno.h>
#include <pj/log.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>


#if defined(PJMEDIA_HAS_VID_MEDIACODEC_CODEC) && \
            PJMEDIA_HAS_VID_MEDIACODEC_CODEC != 0 && \
    defined(PJMEDIA_HAS_VIDEO) && (PJMEDIA_HAS_VIDEO != 0)

#include <media/NdkMediaCodec.h>
#include <media/NdkMediaFormat.h>
#include <pjmedia-codec/h264_packetizer.h>
#include <pjmedia/vid_codec_util.h>
#include <pjmedia/errno.h>
#include <pj/log.h>
#include <pjmedia/format.h>


#define THIS_FILE        "vid_mediacodec.c"


#  define DEFAULT_WIDTH        640
#  define DEFAULT_HEIGHT    720

#define DEFAULT_FPS        15
#define DEFAULT_AVG_BITRATE    (DEFAULT_WIDTH*DEFAULT_HEIGHT*DEFAULT_FPS/16)
#define DEFAULT_MAX_BITRATE    (DEFAULT_WIDTH*DEFAULT_HEIGHT*DEFAULT_FPS/16)

#define MAX_RX_WIDTH        1200
#define MAX_RX_HEIGHT        1200

#define TIMEOUT_US 0

/*2019-09-24 09:08:12.944 31519-31667/com.ruixun.test1 E/sipstack: 17:08:12.944       vid_mediacodec.c  encode : whole_frm.size=26, enc_buf_is_keyframe=0
 * Factory operations.
 */
static pj_status_t mediacodec_test_alloc(pjmedia_vid_codec_factory *factory,
                                         const pjmedia_vid_codec_info *info);

static pj_status_t mediacodec_default_attr(pjmedia_vid_codec_factory *factory,
                                           const pjmedia_vid_codec_info *info,
                                           pjmedia_vid_codec_param *attr);

static pj_status_t mediacodec_enum_info(pjmedia_vid_codec_factory *factory,
                                        unsigned *count,
                                        pjmedia_vid_codec_info codecs[]);

static pj_status_t mediacodec_alloc_codec(pjmedia_vid_codec_factory *factory,
                                          const pjmedia_vid_codec_info *info,
                                          pjmedia_vid_codec **p_codec);

static pj_status_t mediacodec_dealloc_codec(pjmedia_vid_codec_factory *factory,
                                            pjmedia_vid_codec *codec);


/*
 * Codec operations
 */
static pj_status_t mediacodec_codec_init(pjmedia_vid_codec *codec,
                                         pj_pool_t *pool);

static pj_status_t mediacodec_codec_open(pjmedia_vid_codec *codec,
                                         pjmedia_vid_codec_param *param);

static pj_status_t mediacodec_codec_close(pjmedia_vid_codec *codec);

static pj_status_t mediacodec_codec_modify(pjmedia_vid_codec *codec,
                                           const pjmedia_vid_codec_param *param);

static pj_status_t mediacodec_codec_get_param(pjmedia_vid_codec *codec,
                                              pjmedia_vid_codec_param *param);

static pj_status_t mediacodec_codec_encode_begin(pjmedia_vid_codec *codec,
                                                 const pjmedia_vid_encode_opt *opt,
                                                 const pjmedia_frame *input,
                                                 unsigned out_size,
                                                 pjmedia_frame *output,
                                                 pj_bool_t *has_more);

static pj_status_t mediacodec_codec_encode_more(pjmedia_vid_codec *codec,
                                                unsigned out_size,
                                                pjmedia_frame *output,
                                                pj_bool_t *has_more);

static pj_status_t mediacodec_codec_decode(pjmedia_vid_codec *codec,
                                           pj_size_t count,
                                           pjmedia_frame packets[],
                                           unsigned out_size,
                                           pjmedia_frame *output);

/* Definition for Video Mediacodec codecs operations. */
static pjmedia_vid_codec_op mediacodec_codec_op =
        {
                &mediacodec_codec_init,
                &mediacodec_codec_open,
                &mediacodec_codec_close,
                &mediacodec_codec_modify,
                &mediacodec_codec_get_param,
                &mediacodec_codec_encode_begin,
                &mediacodec_codec_encode_more,
                &mediacodec_codec_decode,
                NULL
        };

/* Definition for Video Toolbox codecs factory operations. */
static pjmedia_vid_codec_factory_op mediacodec_factory_op =
        {
                &mediacodec_test_alloc,
                &mediacodec_default_attr,
                &mediacodec_enum_info,
                &mediacodec_alloc_codec,
                &mediacodec_dealloc_codec
        };

/* Mediacodec codecs factory */
static struct mediacodec_factory {
    pjmedia_vid_codec_factory base;
    pjmedia_vid_codec_mgr *mgr;
    pj_pool_factory *pf;
    pj_pool_t *pool;
} mediacodec_factory;

typedef struct mediacodec_codec_data {
    pj_pool_t *pool;
    pjmedia_vid_codec_param *prm;
    pj_bool_t whole;
    pjmedia_h264_packetizer *pktz;

    /* Encoder state */
    AMediaCodec *enc;
    pj_bool_t enc_buf_is_keyframe;
    unsigned enc_input_size;
    unsigned enc_frame_size;
    pj_uint8_t *enc_buf;
    unsigned enc_buf_size;
    unsigned enc_processed;

    pj_bool_t has_sets;
    pj_uint8_t sets[5000];
    unsigned setslen;

    /* Decoder state */
    AMediaCodec *dec;
    pj_uint8_t *dec_buf;
    unsigned dec_buf_size;

} mediacodec_codec_data;

/***********************************************************/
PJ_DEF(pj_status_t) pjmedia_codec_vid_mediacodec_init(pjmedia_vid_codec_mgr *mgr,
                                                      pj_pool_factory *pf) {
    const pj_str_t h264_name = {(char *) "H264", 4};
    pj_status_t status;

    if (mediacodec_factory.pool != NULL) {
        /* Already initialized. */
        return PJ_SUCCESS;
    }

    if (!mgr) mgr = pjmedia_vid_codec_mgr_instance();
    PJ_ASSERT_RETURN(mgr, PJ_EINVAL);

    /* Create Mediacodec codec factory. */
    mediacodec_factory.base.op = &mediacodec_factory_op;
    mediacodec_factory.base.factory_data = NULL;
    mediacodec_factory.mgr = mgr;
    mediacodec_factory.pf = pf;
    mediacodec_factory.pool = pj_pool_create(pf, "mediacodecfactory", 256, 256, NULL);
    if (!mediacodec_factory.pool)
        return PJ_ENOMEM;

    /* Registering format match for SDP negotiation */
    status = pjmedia_sdp_neg_register_fmt_match_cb(
            &h264_name,
            &pjmedia_vid_codec_h264_match_sdp);
    if (status != PJ_SUCCESS)
        goto on_error;

    /* Register codec factory to codec manager. */
    status = pjmedia_vid_codec_mgr_register_factory(mgr,
                                                    &mediacodec_factory.base);
    if (status != PJ_SUCCESS)
        goto on_error;

    PJ_LOG(4, (THIS_FILE, "Android MediaCodec codec initialized"));

    /* Done. */
    return PJ_SUCCESS;

    on_error:
    PJ_LOG(4, (THIS_FILE, "MediaCodec codec error :%d", status));
    pj_pool_release(mediacodec_factory.pool);
    mediacodec_factory.pool = NULL;
    return status;
}

PJ_DEF(pj_status_t) pjmedia_codec_vid_mediacodec_deinit(void) {
    pj_status_t status = PJ_SUCCESS;

    if (mediacodec_factory.pool == NULL) {
        /* Already deinitialized */
        return PJ_SUCCESS;
    }

    /* Unregister MediaCodec codecs factory. */
    status = pjmedia_vid_codec_mgr_unregister_factory(mediacodec_factory.mgr,
                                                      &mediacodec_factory.base);

    /* Destroy pool. */
    pj_pool_release(mediacodec_factory.pool);
    mediacodec_factory.pool = NULL;

    return status;
}

static pj_status_t mediacodec_test_alloc(pjmedia_vid_codec_factory *factory,
                                         const pjmedia_vid_codec_info *info) {
    PJ_ASSERT_RETURN(factory == &mediacodec_factory.base, PJ_EINVAL);

    if (info->fmt_id == PJMEDIA_FORMAT_H264 &&
        info->pt != 0) {
        return PJ_SUCCESS;
    }

    return PJMEDIA_CODEC_EUNSUP;
}


static pj_status_t mediacodec_default_attr(pjmedia_vid_codec_factory *factory,
                                           const pjmedia_vid_codec_info *info,
                                           pjmedia_vid_codec_param *attr) {
    PJ_ASSERT_RETURN(factory == &mediacodec_factory.base, PJ_EINVAL);
    PJ_ASSERT_RETURN(info && attr, PJ_EINVAL);

    pj_bzero(attr, sizeof(pjmedia_vid_codec_param));

    attr->dir = PJMEDIA_DIR_ENCODING_DECODING;
    attr->packing = PJMEDIA_VID_PACKING_PACKETS;

    /* Encoded format */
    pjmedia_format_init_video(&attr->enc_fmt, PJMEDIA_FORMAT_H264,
                              DEFAULT_WIDTH, DEFAULT_HEIGHT,
                              DEFAULT_FPS, 1);

    /* Decoded format */
    pjmedia_format_init_video(&attr->dec_fmt, PJMEDIA_FORMAT_I420,
                              DEFAULT_WIDTH, DEFAULT_HEIGHT,
                              DEFAULT_FPS, 1);

    /* Decoding fmtp */
    attr->dec_fmtp.cnt = 2;
    attr->dec_fmtp.param[0].name = pj_str((char *) "profile-level-id");
    attr->dec_fmtp.param[0].val = pj_str((char *) "42e01e");
    attr->dec_fmtp.param[1].name = pj_str((char *) " packetization-mode");
    attr->dec_fmtp.param[1].val = pj_str((char *) "1");

    /* Bitrate */
    attr->enc_fmt.det.vid.avg_bps = DEFAULT_AVG_BITRATE;
    attr->enc_fmt.det.vid.max_bps = DEFAULT_MAX_BITRATE;

    /* Encoding MTU */
    attr->enc_mtu = PJMEDIA_MAX_VID_PAYLOAD_SIZE;

    //attr->ignore_fmtp = 1;

    return PJ_SUCCESS;
}

static pj_status_t mediacodec_enum_info(pjmedia_vid_codec_factory *factory,
                                        unsigned *count,
                                        pjmedia_vid_codec_info info[]) {
    PJ_ASSERT_RETURN(info && *count > 0, PJ_EINVAL);
    PJ_ASSERT_RETURN(factory == &mediacodec_factory.base, PJ_EINVAL);

    *count = 1;
    info->fmt_id = PJMEDIA_FORMAT_H264;
    info->pt = PJMEDIA_RTP_PT_H264;
    info->encoding_name = pj_str((char *) "H264");
    info->encoding_desc = pj_str((char *) "Android MediaCodec codec");
    info->clock_rate = 90000;
    info->dir = PJMEDIA_DIR_ENCODING_DECODING;
    info->dec_fmt_id_cnt = 1;
    info->dec_fmt_id[0] = PJMEDIA_FORMAT_I420;
    info->packings = PJMEDIA_VID_PACKING_PACKETS |
                     PJMEDIA_VID_PACKING_WHOLE;
    info->fps_cnt = 3;
    info->fps[0].num = 15;
    info->fps[0].denum = 1;
    info->fps[1].num = 25;
    info->fps[1].denum = 1;
    info->fps[2].num = 30;
    info->fps[2].denum = 1;

    return PJ_SUCCESS;
}

static pj_status_t mediacodec_alloc_codec(pjmedia_vid_codec_factory *factory,
                                          const pjmedia_vid_codec_info *info,
                                          pjmedia_vid_codec **p_codec) {

    pj_pool_t *pool;
    pjmedia_vid_codec *codec;
    mediacodec_codec_data *mediacodec_data;

    PJ_ASSERT_RETURN(factory == &mediacodec_factory.base && info && p_codec,
                     PJ_EINVAL);

    *p_codec = NULL;

    pool = pj_pool_create(mediacodec_factory.pf, "mediacodec%p", 512, 512, NULL);
    if (!pool)
        return PJ_ENOMEM;

    /* codec instance */
    codec = PJ_POOL_ZALLOC_T(pool, pjmedia_vid_codec);
    codec->factory = factory;
    codec->op = &mediacodec_codec_op;

    /* codec data */
    mediacodec_data = PJ_POOL_ZALLOC_T(pool, mediacodec_codec_data);
    mediacodec_data->pool = pool;
    codec->codec_data = mediacodec_data;

    *p_codec = codec;
    return PJ_SUCCESS;

}

static pj_status_t mediacodec_dealloc_codec(pjmedia_vid_codec_factory *factory,
                                            pjmedia_vid_codec *codec) {
    mediacodec_codec_data *mediacodec_data;

    PJ_ASSERT_RETURN(codec, PJ_EINVAL);

    PJ_UNUSED_ARG(factory);

    mediacodec_data = (mediacodec_codec_data *) codec->codec_data;
    pj_pool_release(mediacodec_data->pool);
    return PJ_SUCCESS;
}


static pj_status_t mediacodec_codec_init(pjmedia_vid_codec *codec,
                                         pj_pool_t *pool) {
    PJ_ASSERT_RETURN(codec && pool, PJ_EINVAL);
    PJ_UNUSED_ARG(codec);
    PJ_UNUSED_ARG(pool);
    return PJ_SUCCESS;
}

static pj_status_t mediacodec_decodec_open(mediacodec_codec_data *mediacodec_data) {
    mediacodec_data->dec = AMediaCodec_createDecoderByType("video/avc");
    AMediaFormat *defmt = AMediaFormat_new();
    AMediaFormat_setString(defmt, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(defmt, AMEDIAFORMAT_KEY_WIDTH,
                          mediacodec_data->prm->dec_fmt.det.vid.size.w);
    AMediaFormat_setInt32(defmt, AMEDIAFORMAT_KEY_HEIGHT,
                          mediacodec_data->prm->dec_fmt.det.vid.size.h);

    pj_status_t mediastatus = AMediaCodec_configure(mediacodec_data->dec, defmt, NULL, NULL, 0);
    if (mediastatus != 0) {
        PJ_LOG(4,
               (THIS_FILE, "decode : AMediaCodec_configure() failed with error %i ", (int) mediastatus));
    } else {
        if ((AMediaCodec_start(mediacodec_data->dec)) != AMEDIA_OK) {
            PJ_LOG(4, (THIS_FILE, "decode : AMediaCodec_start: Could not start decoder."));
        } else {
            PJ_LOG(4, (THIS_FILE, "decode : AMediaCodec_start: decoder successfully started"));
        }
    }
    return mediastatus;
}


static pj_status_t mediacodec_codec_open(pjmedia_vid_codec *codec,
                                         pjmedia_vid_codec_param *codec_param) {
    mediacodec_codec_data *mediacodec_data;
    pjmedia_vid_codec_param *param;
    pjmedia_h264_packetizer_cfg pktz_cfg;
    pjmedia_vid_codec_h264_fmtp h264_fmtp;
    pj_status_t status;

    PJ_ASSERT_RETURN(codec && codec_param, PJ_EINVAL);
    PJ_LOG(5, (THIS_FILE, "Opening codec.."));

    mediacodec_data = (mediacodec_codec_data *) codec->codec_data;
    mediacodec_data->prm = pjmedia_vid_codec_param_clone(mediacodec_data->pool,
                                                         codec_param);
    param = mediacodec_data->prm;

    /* Parse remote fmtp */
    pj_bzero(&h264_fmtp, sizeof(h264_fmtp));
    status = pjmedia_vid_codec_h264_parse_fmtp(&param->enc_fmtp, &h264_fmtp);
    if (status != PJ_SUCCESS)
        return status;

    /* Apply SDP fmtp to format in codec param */
    if (!param->ignore_fmtp) {
        status = pjmedia_vid_codec_h264_apply_fmtp(param);
        if (status != PJ_SUCCESS)
            return status;
    }

    pj_bzero(&pktz_cfg, sizeof(pktz_cfg));
    pktz_cfg.mtu = param->enc_mtu;
    pktz_cfg.unpack_nal_start = 4;

    /* Packetization mode */

    if (h264_fmtp.packetization_mode == 0)
        pktz_cfg.mode = PJMEDIA_H264_PACKETIZER_MODE_SINGLE_NAL;
    else if (h264_fmtp.packetization_mode == 1)
        pktz_cfg.mode = PJMEDIA_H264_PACKETIZER_MODE_NON_INTERLEAVED;
    else
        return PJ_ENOTSUP;
    pktz_cfg.mode = PJMEDIA_H264_PACKETIZER_MODE_NON_INTERLEAVED;

    status = pjmedia_h264_packetizer_create(mediacodec_data->pool, &pktz_cfg,
                                            &mediacodec_data->pktz);
    if (status != PJ_SUCCESS)
        return status;

    mediacodec_data->whole = (param->packing == PJMEDIA_VID_PACKING_WHOLE);

    /*
     * Encoder
     */
    mediacodec_data->enc = AMediaCodec_createEncoderByType("video/avc");
    if (mediacodec_data->enc == NULL) {
        PJ_LOG(4, (THIS_FILE, "encode : could not create Encoder"));
    }

    AMediaFormat *enfmt = AMediaFormat_new();
    AMediaFormat_setString(enfmt, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(enfmt, AMEDIAFORMAT_KEY_WIDTH, param->enc_fmt.det.vid.size.w);
    AMediaFormat_setInt32(enfmt, AMEDIAFORMAT_KEY_HEIGHT, param->enc_fmt.det.vid.size.h);

    AMediaFormat_setInt32(enfmt, AMEDIAFORMAT_KEY_COLOR_FORMAT, 19);
    AMediaFormat_setInt32(enfmt, AMEDIAFORMAT_KEY_BIT_RATE, param->enc_fmt.det.vid.max_bps);
    AMediaFormat_setInt32(enfmt, AMEDIAFORMAT_KEY_FRAME_RATE, param->enc_fmt.det.vid.fps.num);
    AMediaFormat_setInt32(enfmt, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 1);

    media_status_t mediastatus = AMediaCodec_configure(mediacodec_data->enc, enfmt, NULL, NULL, 1);
    if (mediastatus != 0) {
        PJ_LOG(4,
               (THIS_FILE, "encode : AMediaCodec_configure() failed with error %i for format %u", (int) status, 21));
    } else {
        if ((AMediaCodec_start(mediacodec_data->enc)) != AMEDIA_OK) {
            PJ_LOG(4, (THIS_FILE, "encode : AMediaCodec_start: Could not start encoder."));
        } else {
            PJ_LOG(4, (THIS_FILE, "encode : AMediaCodec_start: encoder successfully started"));
        }
    }

    AMediaFormat_delete(enfmt);

    /*
     * Decoder
     */
    mediacodec_data->dec = AMediaCodec_createDecoderByType("video/avc");
    AMediaFormat *defmt = AMediaFormat_new();
    AMediaFormat_setString(defmt, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(defmt, AMEDIAFORMAT_KEY_WIDTH, param->dec_fmt.det.vid.size.w);
    AMediaFormat_setInt32(defmt, AMEDIAFORMAT_KEY_HEIGHT, param->dec_fmt.det.vid.size.h);


    PJ_LOG(4,
           (THIS_FILE, "decode : AMediaFormat:MIME[%s],W[%d],H[%d]", "video/avc", param->dec_fmt.det.vid.size.w, param->dec_fmt.det.vid.size.h));

    mediastatus = AMediaCodec_configure(mediacodec_data->dec, defmt, NULL, NULL, 0);
    if (mediastatus != 0) {
        PJ_LOG(4,
               (THIS_FILE, "decode : AMediaCodec_configure() failed with error %i ", (int) mediastatus));
    } else {
        if ((AMediaCodec_start(mediacodec_data->dec)) != AMEDIA_OK) {
            PJ_LOG(4, (THIS_FILE, "decode : AMediaCodec_start: Could not start decoder."));
        } else {
            PJ_LOG(4, (THIS_FILE, "decode : AMediaCodec_start: decoder successfully started"));
        }
    }


    AMediaFormat_delete(defmt);

    /* Alloc buffers if needed */


    mediacodec_data->whole = (param->packing == PJMEDIA_VID_PACKING_WHOLE);
    if (1) {
        /* Init format info and apply-param of encoder */
        const pjmedia_video_format_info *enc_vfi;
        pjmedia_video_apply_fmt_param enc_vafp;

        enc_vfi = pjmedia_get_video_format_info(NULL, codec_param->dec_fmt.id);
        if (!enc_vfi)
            return PJ_EINVAL;

        pj_bzero(&enc_vafp, sizeof(enc_vafp));
        enc_vafp.size = codec_param->enc_fmt.det.vid.size;
        enc_vafp.buffer = NULL;
        status = (*enc_vfi->apply_fmt)(enc_vfi, &enc_vafp);
        if (status != PJ_SUCCESS)
            return status;

        mediacodec_data->enc_input_size = enc_vafp.framebytes;
        if (!mediacodec_data->whole) {
            mediacodec_data->enc_buf_size = (unsigned) enc_vafp.framebytes;
            mediacodec_data->enc_buf = pj_pool_alloc(mediacodec_data->pool,
                                                     mediacodec_data->enc_buf_size);
            PJ_LOG(4, (THIS_FILE, "encode : enc_buf_size = %d", mediacodec_data->enc_buf_size));
        }
    }
    if (!mediacodec_data->whole) {


        mediacodec_data->dec_buf_size = (MAX_RX_WIDTH * MAX_RX_HEIGHT * 3 >> 1) + (MAX_RX_WIDTH);
        mediacodec_data->dec_buf = (pj_uint8_t *) pj_pool_alloc(mediacodec_data->pool,
                                                                mediacodec_data->dec_buf_size);
    }


    pj_memcpy(codec_param, param, sizeof(*codec_param));

    return PJ_SUCCESS;
}

static pj_status_t mediacodec_codec_close(pjmedia_vid_codec *codec) {
    struct mediacodec_codec_data *mediacodec_data;

    PJ_ASSERT_RETURN(codec, PJ_EINVAL);

    mediacodec_data = (mediacodec_codec_data *) codec->codec_data;

    if (mediacodec_data->enc) {
        AMediaCodec_stop(mediacodec_data->enc);
        AMediaCodec_delete(mediacodec_data->enc);
        mediacodec_data->enc = NULL;
        PJ_LOG(4, (THIS_FILE, "encode : AMediaCodec_delete"));
    }

    if (mediacodec_data->dec) {
        AMediaCodec_stop(mediacodec_data->dec);
        AMediaCodec_delete(mediacodec_data->dec);
        mediacodec_data->dec = NULL;
        PJ_LOG(4, (THIS_FILE, "decode : AMediaCodec_delete"));
    }


    return PJ_SUCCESS;
}

static pj_status_t mediacodec_codec_modify(pjmedia_vid_codec *codec,
                                           const pjmedia_vid_codec_param *param) {
    PJ_ASSERT_RETURN(codec && param, PJ_EINVAL);
    PJ_UNUSED_ARG(codec);
    PJ_UNUSED_ARG(param);
    return PJ_EINVALIDOP;
}

static pj_status_t mediacodec_codec_get_param(pjmedia_vid_codec *codec,
                                              pjmedia_vid_codec_param *param) {
    struct mediacodec_codec_data *mediacodec_data;

    PJ_ASSERT_RETURN(codec && param, PJ_EINVAL);

    mediacodec_data = (mediacodec_codec_data *) codec->codec_data;
    pj_memcpy(param, mediacodec_data->prm, sizeof(*param));

    return PJ_SUCCESS;
}

static unsigned long long timeGetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (unsigned long long) (tv.tv_sec) * 1000000 + tv.tv_usec;
}

static pj_status_t mediacodec_codec_encode_whole(pjmedia_vid_codec *codec,
                                                 const pjmedia_vid_encode_opt *opt,
                                                 const pjmedia_frame *input,
                                                 unsigned output_buf_len,
                                                 pjmedia_frame *output) {
    mediacodec_codec_data *mediacodec_data = (mediacodec_codec_data *) codec->codec_data;
    ssize_t ibufidx, obufidx;
    AMediaCodecBufferInfo info;
    size_t obufsize = 0;
    size_t ibufsize = 0;

    output->size = 0;
    output->type = PJMEDIA_FRAME_TYPE_NONE;

    /*First queue input image*/
    uint8_t *buf;

    ibufidx = AMediaCodec_dequeueInputBuffer(mediacodec_data->enc, TIMEOUT_US);
    if (ibufidx >= 0) {
        buf = AMediaCodec_getInputBuffer(mediacodec_data->enc, ibufidx, &ibufsize);
        if (buf) {
            memcpy(buf, input->buf, input->size);
            ibufsize = input->size;
            unsigned long long curTime = timeGetTime();

            PJ_LOG(4,
                   (THIS_FILE, "encode : AMediaCodec_queueInputBuffer: ibufidx[%d] bufsize[%d] curTime[%lld]", ibufidx, ibufsize));
            AMediaCodec_queueInputBuffer(mediacodec_data->enc, ibufidx, 0, ibufsize, curTime, 0);
        } else {
            PJ_LOG(4, (THIS_FILE, "encode : obtained InputBuffer, but no address."));
        }
    } else if (ibufidx == AMEDIA_ERROR_UNKNOWN) {
        PJ_LOG(4, (THIS_FILE, "encode : AMediaCodec_dequeueInputBuffer() had an exception"));
    }

    if ((obufidx = AMediaCodec_dequeueOutputBuffer(mediacodec_data->enc, &info, TIMEOUT_US)) >= 0) {
        PJ_LOG(4,
               (THIS_FILE, "encode : AMediaCodec_dequeueOutputBuffer: obufidx=%d, info.size=%d, info.flags=%d, info.offset=%d, info.presentationTimeUs=%lld ",
                       obufidx, info.size, info.flags, info.offset, info.presentationTimeUs));
        char *oBuf = AMediaCodec_getOutputBuffer(mediacodec_data->enc, obufidx, &obufsize);
        if (oBuf) {
            PJ_LOG(4,
                   (THIS_FILE, "encode : AMediaCodec_getOutputBuffer: obufidx=%d, bufsize=%d,info.size=%d output_buf_len=%d", obufidx, obufsize, info.size, output_buf_len));
            memcpy(output->buf, oBuf, info.size);
            output->type = PJMEDIA_FRAME_TYPE_VIDEO;
            output->size = info.size;
            output->timestamp = input->timestamp;
            if (info.flags == 1) {
                output->bit_info |= PJMEDIA_VID_FRM_KEYFRAME;
            }

            AMediaCodec_releaseOutputBuffer(mediacodec_data->enc, obufidx, false);
        }
        if (obufidx == AMEDIA_ERROR_UNKNOWN) {
            PJ_LOG(4,
                   (THIS_FILE, "encode : AMediaCodec_dequeueOutputBuffer() had an exception, MediaCodec is lost"));
        }
    }

    return PJ_SUCCESS;
}

static pj_status_t mediacodec_codec_encode_begin(pjmedia_vid_codec *codec,
                                                 const pjmedia_vid_encode_opt *opt,
                                                 const pjmedia_frame *input,
                                                 unsigned out_size,
                                                 pjmedia_frame *output,
                                                 pj_bool_t *has_more) {
    mediacodec_codec_data *mediacodec_data = (mediacodec_codec_data *) codec->codec_data;
    pj_status_t status;

    *has_more = PJ_FALSE;

    if (mediacodec_data->whole) {
        status = mediacodec_codec_encode_whole(codec, opt, input, out_size,
                                               output);
    } else {
        pjmedia_frame whole_frm;
        const pj_uint8_t *payload;
        pj_size_t payload_len;

        pj_bzero(&whole_frm, sizeof(whole_frm));
        whole_frm.buf = mediacodec_data->enc_buf;
        whole_frm.size = mediacodec_data->enc_buf_size;
        status = mediacodec_codec_encode_whole(codec, opt, input,
                                               (unsigned) whole_frm.size,
                                               &whole_frm);
        if (status != PJ_SUCCESS)
            return status;

        if (whole_frm.size <= 0)
            return status;

        // sps pps
        if (mediacodec_data->has_sets == false) {
            mediacodec_data->has_sets = true;
            memcpy(mediacodec_data->sets, whole_frm.buf, whole_frm.size);
            mediacodec_data->setslen = whole_frm.size;
        }

        mediacodec_data->enc_buf_is_keyframe = (whole_frm.bit_info &
                                                PJMEDIA_VID_FRM_KEYFRAME);
        mediacodec_data->enc_frame_size = (unsigned) whole_frm.size;
        mediacodec_data->enc_processed = 0;

        if (mediacodec_data->enc_buf_is_keyframe)
            //if(false)
        {
            memmove(mediacodec_data->enc_buf + mediacodec_data->setslen, mediacodec_data->enc_buf,
                    mediacodec_data->enc_frame_size);
            memcpy(mediacodec_data->enc_buf, mediacodec_data->sets, mediacodec_data->setslen);
            mediacodec_data->enc_frame_size += mediacodec_data->setslen;
        }

        status = pjmedia_h264_packetize(mediacodec_data->pktz,
                                        (pj_uint8_t *) mediacodec_data->enc_buf,
                                        mediacodec_data->enc_frame_size,
                                        &mediacodec_data->enc_processed,
                                        &payload, &payload_len);
        if (status != PJ_SUCCESS)
            return status;

        if (out_size < payload_len)
            return PJMEDIA_CODEC_EFRMTOOSHORT;

        output->type = PJMEDIA_FRAME_TYPE_VIDEO;
        pj_memcpy(output->buf, payload, payload_len);
        output->size = payload_len;

        if (mediacodec_data->enc_buf_is_keyframe)
            output->bit_info |= PJMEDIA_VID_FRM_KEYFRAME;

        *has_more = (mediacodec_data->enc_processed < mediacodec_data->enc_frame_size);
    }

    return status;
}

static pj_status_t mediacodec_codec_encode_more(pjmedia_vid_codec *codec,
                                                unsigned out_size,
                                                pjmedia_frame *output,
                                                pj_bool_t *has_more) {
    mediacodec_codec_data *mediacodec_data = (mediacodec_codec_data *) codec->codec_data;
    const pj_uint8_t *payload;
    pj_size_t payload_len;
    pj_status_t status;

    *has_more = PJ_FALSE;

    if (mediacodec_data->enc_processed >= mediacodec_data->enc_frame_size) {
        /* No more frame */
        return PJ_EEOF;
    }

    status = pjmedia_h264_packetize(mediacodec_data->pktz, (pj_uint8_t *) mediacodec_data->enc_buf,
                                    mediacodec_data->enc_frame_size,
                                    &mediacodec_data->enc_processed,
                                    &payload, &payload_len);
    if (status != PJ_SUCCESS)
        return status;

    if (out_size < payload_len)
        return PJMEDIA_CODEC_EFRMTOOSHORT;

    output->type = PJMEDIA_FRAME_TYPE_VIDEO;
    pj_memcpy(output->buf, payload, payload_len);
    output->size = payload_len;

    if (mediacodec_data->enc_buf_is_keyframe)
        output->bit_info |= PJMEDIA_VID_FRM_KEYFRAME;

    *has_more = (mediacodec_data->enc_processed < mediacodec_data->enc_frame_size);

    return PJ_SUCCESS;

}


static pj_status_t mediacodec_codec_decode_whole(pjmedia_vid_codec *codec,
                                                 const pjmedia_frame *input,
                                                 unsigned output_buf_len,
                                                 pjmedia_frame *output) {
    mediacodec_codec_data *mediacodec_data = (mediacodec_codec_data *) codec->codec_data;

    ssize_t oBufidx = -1;
    size_t ibufsize = 0;
    size_t obufsize = 0;
    AMediaCodecBufferInfo info;
    uint8_t *buf = NULL;
    ssize_t iBufidx = -1;

    output->size = 0;

    /*First put our H264 bitstream into the decoder*/
    if (input) {
        iBufidx = AMediaCodec_dequeueInputBuffer(mediacodec_data->dec, TIMEOUT_US);
        PJ_LOG(4, (THIS_FILE, "decode : iBufidx %d %d", iBufidx, input->size));
        if (iBufidx >= 0) {
            buf = AMediaCodec_getInputBuffer(mediacodec_data->dec, iBufidx, &ibufsize);


            if (buf) {
                ibufsize = input->size;
                memcpy(buf, input->buf, ibufsize);
            }

            AMediaCodec_queueInputBuffer(mediacodec_data->dec, iBufidx, 0, ibufsize, timeGetTime(),
                                         0);
        } else if (iBufidx == -1) {
            /*
            * This is a problematic case because we can't wait the decoder to be ready, otherwise we'll freeze the entire
            * video thread.
            * We have no other option to drop the frame, and retry later, but with an I-frame of course.
            **/

        }


        /*secondly try to get decoded frames from the decoder, this is performed every tick*/
        oBufidx = AMediaCodec_dequeueOutputBuffer(mediacodec_data->dec, &info, TIMEOUT_US);
        PJ_LOG(4, (THIS_FILE, "decode : oBufidx %d", oBufidx));
        if (oBufidx >= 0) {
            AMediaFormat *format;
            int color = 0;

            PJ_LOG(4,
                   (THIS_FILE, "decode : info: offset[%d] flags[%d] size[%d] presentationTimeUs[%ld]",
                           info.offset, info.flags, info.size, info.presentationTimeUs));

            uint8_t *buf = AMediaCodec_getOutputBuffer(mediacodec_data->dec, oBufidx, &obufsize);

            if (buf == NULL) {
                PJ_LOG(4, (THIS_FILE, "decode : AMediaCodec_getOutputBuffer() returned NULL"));
            } else {
                int width = 0, height = 0;
                format = AMediaCodec_getOutputFormat(mediacodec_data->dec);
                if (format != NULL) {
                    AMediaFormat_getInt32(format, "width", &width);
                    AMediaFormat_getInt32(format, "height", &height);
                    AMediaFormat_getInt32(format, "color-format", &color);

                    AMediaFormat_delete(format);
                }
                if (mediacodec_data->prm->dec_fmt.det.vid.size.w != width ||
                    mediacodec_data->prm->dec_fmt.det.vid.size.h != height) {
                    mediacodec_data->prm->dec_fmt.det.vid.size.w = width;
                    mediacodec_data->prm->dec_fmt.det.vid.size.h = height;

                    pjmedia_event event;
                    /* Broadcast format changed event */
                    pjmedia_event_init(&event, PJMEDIA_EVENT_FMT_CHANGED,
                                       &input->timestamp, codec);
                    event.data.fmt_changed.dir = PJMEDIA_DIR_DECODING;
                    pjmedia_format_copy(&event.data.fmt_changed.new_fmt,
                                        &mediacodec_data->prm->dec_fmt);
                    pjmedia_event_publish(NULL, codec, &event,
                                          PJMEDIA_EVENT_PUBLISH_DEFAULT);
                }
                if (width != 0 && height != 0) {
                    PJ_LOG(4,
                           (THIS_FILE, "decode : buf[%d]info.size[%d]size[%dx%d]color-format[%d]",
                                   obufsize, info.size, width, height, color));

                    memcpy(output->buf, buf, info.size);
                    output->size = info.size;

                } else {
                    PJ_LOG(4, (THIS_FILE, "decode : width and height are not known !"));
                }
            }
            AMediaCodec_releaseOutputBuffer(mediacodec_data->dec, oBufidx, false);

        } else if (oBufidx == AMEDIA_ERROR_UNKNOWN) {
            PJ_LOG(4, (THIS_FILE, "decode : AMedia Codec_dequeueOutputBuffer() had an exception"));
            AMediaCodec_stop(mediacodec_data->dec);
            PJ_LOG(4, (THIS_FILE, "decode : AMediaCodec_stop"));
            mediacodec_decodec_open(mediacodec_data);
        } else {
            PJ_LOG(4,
                   (THIS_FILE, "decode : AMediaCodec_dequeueOutputBuffer() oBufidx=%d", oBufidx));
        }

    }


    return PJ_SUCCESS;
}

static pj_status_t mediacodec_codec_decode(pjmedia_vid_codec *codec,
                                           pj_size_t pkt_count,
                                           pjmedia_frame packets[],
                                           unsigned out_size,
                                           pjmedia_frame *output) {
    mediacodec_codec_data *mediacodec_data = (mediacodec_codec_data *) codec->codec_data;
    pj_status_t status;

    PJ_ASSERT_RETURN(codec && pkt_count > 0 && packets && output,
                     PJ_EINVAL);

    if (mediacodec_data->whole) {
        pj_assert(pkt_count == 1);
        return mediacodec_codec_decode_whole(codec, &packets[0], out_size, output);
    } else {
        pjmedia_frame whole_frm;
        unsigned whole_len = 0;
        unsigned i;

        for (i = 0; i < pkt_count; ++i) {
            if (whole_len + packets[i].size > mediacodec_data->dec_buf_size) {
                PJ_LOG(5, (THIS_FILE, "decode : Decoding buffer overflow"));
                break;
            }


            status = pjmedia_h264_unpacketize(mediacodec_data->pktz,
                                              (pj_uint8_t *) packets[i].buf,
                                              packets[i].size,
                                              mediacodec_data->dec_buf,
                                              mediacodec_data->dec_buf_size,
                                              &whole_len);
            if (status != PJ_SUCCESS) {
                PJ_PERROR(5, (THIS_FILE, status, "decode : Unpacketize error"));
                continue;
            }
        }

        whole_frm.buf = mediacodec_data->dec_buf;
        whole_frm.size = whole_len;
        whole_frm.timestamp = output->timestamp = packets[i].timestamp;
        whole_frm.bit_info = 0;

        return mediacodec_codec_decode_whole(codec, &whole_frm, out_size, output);
    }


}


#endif    /* PJMEDIA_HAS_VID_MEDIACODEC_CODEC */