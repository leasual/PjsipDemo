#define PJ_CONFIG_ANDROID 1
#define PJMEDIA_HAS_LEGACY_SOUND_API 0
#define PJMEDIA_HAS_L16_CODEC 0
#define PJMEDIA_HAS_ILBC_CODEC 0
#define PJMEDIA_HAS_G722_CODEC 0
#define PJMEDIA_HAS_G7221_CODEC 0
#define PJMEDIA_HAS_OPUS_CODEC 0
#define PJMEDIA_STREAM_ENABLE_KA 1
#define PJMEDIA_HAS_SPEEX_AEC 1
#define PJMEDIA_HAS_WEBRTC_AEC 1
#define PJMEDIA_WEBRTC_AEC_USE_MOBILE 0
#define SHOW_DELAY_METRICS 1
#define PJMEDIA_HAS_SRTP 1

#define ARM

#include <pj/config_site_sample.h>
#define PJMEDIA_HAS_VIDEO 1
#define PJMEDIA_HAS_VID_MEDIACODEC_CODEC 1
#define PJMEDIA_AUDIO_DEV_HAS_ANDROID_JNI 1
#define PJMEDIA_AUDIO_DEV_HAS_OPENSL 0
#define PJSIP_AUTH_AUTO_SEND_NEXT 0
#define PJMEDIA_ADVERTISE_RTCP 0
#define PJMEDIA_ADD_BANDWIDTH_TIAS_IN_SDP 0
#define PJMEDIA_ADD_RTPMAP_FOR_STATIC_PT 0
