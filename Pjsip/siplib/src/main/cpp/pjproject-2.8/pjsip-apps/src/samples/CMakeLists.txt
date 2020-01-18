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
        ../pjsystest/
)

LINK_DIRECTORIES(libs/${ANDROID_ABI}/)

set(LINK_LIBRARY pjsua2-lib

#        stdc++

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

find_library(log-lib log)

####  executable  ####

if(0)

set(TEST_SRC
        aectest.c
        auddemo.c
        aviplay.c
        clidemo.c
        confsample.c
        encdec.c
        footprint.c
        httpdemo.c
        icedemo.c
        invtester.c
        jbsim.c
        latency.c
        level.c
        mix.c
        pcaputil.c
        pjsip-perf.c
        pjsua2_demo.cpp
        playfile.c
        playsine.c
        recfile.c
        resampleplay.c
        simple_pjsua.c
        simpleua.c
        sipecho.c
        siprtp.c
        sipstateless.c
        stateful_proxy.c
        stateless_proxy.c
        stereotest.c
        streamutil.c
        strerror.c
        tonegen.c
        vid_codec_test.c
        vid_streamutil.c
#        siprtp_report.c
#        main_rtems.c
#        confbench.c
        )

foreach(mainfile IN LISTS TEST_SRC)
    # Get file name without directory
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
    target_link_libraries( # Specifies the target library.
            ${mainname}
            ${LINK_LIBRARY})
endforeach()

endif()

set(TEST_SRC
        test.cpp
        aectest.c
        simple_pjsua.c)

add_library(module_test
        SHARED
        ${TEST_SRC})
target_link_libraries(module_test
        ${LINK_LIBRARY})


