cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make sample-test ################")

add_definitions( -D_ARM_ )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )
add_definitions( -DPJ_AUTOCONF=1 )

add_definitions( -DPJMEDIA_VIDEO_DEV_HAS_ANDROID_OPENGL=1 )
add_definitions( -DPJMEDIA_VIDEO_DEV_HAS_ANDROID=1 )
add_definitions( -DPJMEDIA_HAS_OPENH264_CODEC=1 )
add_definitions( -DPJMEDIA_HAS_OPENCORE_AMRNB_CODEC=0 )
add_definitions( -DPJMEDIA_HAS_OPENCORE_AMRWB_CODEC=0 )
add_definitions( -DPJMEDIA_RESAMPLE_IMP=PJMEDIA_RESAMPLE_LIBRESAMPLE )
add_definitions( -DPJMEDIA_HAS_LIBYUV=1 )
add_definitions( -DPJMEDIA_HAS_WEBRTC_AEC=1 )
add_definitions( -DPJMEDIA_AUDIO_DEV_HAS_WMME=0 )


include_directories(
        ../../../auto_config/${ANDROID_ABI}
        ../../../pjsip/include
        ../../../pjmedia/include
        ../../../pjlib/include
        ../../../pjlib-util/include
        ../../../pjnath/include
        ../../../third_party/
)

LINK_DIRECTORIES(libs/${ANDROID_ABI}/)


set(LINK_LIBRARY

        stdc++

        pjsua-lib
        pjsip-ua-lib
        pjsip-simple-lib
        pjsip-lib

        pjmedia-codec-lib
        pjmedia-lib

        pjmedia-videodev-lib
        pjmedia-audiodev-lib

        pjmedia-lib

        pjnath-lib
        pjlib-util-lib

        libsrtp.a
        libresample.a
        libgsmcodec.a
        libspeex.a
        libilbccodec.a
        libg7221codec.a
        libyuv.a
        libwebrtc.a
        pjlib-lib
        libopenh264.so

        OpenSLES
        GLESv2
        EGL
        android
        mediandk
        OpenMAXAL

        # Links the target library to the log library
        # included in the NDK.

        ${log-lib})


if(0)
set(LINK_LIBRARY
        pjsua-lib
        pjmedia-lib
        pjlib-util-lib
        ${log-lib})
        endif(0)

find_library(log-lib log)

set(TEST_SRC
        test.cpp
        test.c
        codec_vectors.c
        jbuf_test.c
#        mips_test.c
        rtp_test.c
        sdp_neg_test.c
        sdptest.c
        session_test.c
        vid_codec_test.c
        vid_dev_test.c
        vid_port_test.c
        wsola_test.c
        audio_tool.c)

add_library(pjmedia_test
        SHARED
        ${TEST_SRC})
target_link_libraries(pjmedia_test
        ${LINK_LIBRARY})






