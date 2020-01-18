cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjmedia-audiodev-lib ################")

add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )
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
					../../../../openh264/include
                    ../jni

                    ../../../../jniLibs/armeabi/include
                    ../../../third_party/build/speex
                    ../../../third_party/speex/include
                    ../../../third_party/build/srtp
                    ../../../third_party/srtp/crypto/include
                    ../../../third_party/srtp/include
                    ../../../third_party/yuv/include
                    ../../../third_party/webrtc/src
                    ../../include
                    ../../../pjlib/include
                    ../../../pjlib-util/include
                    ../../../pjmedia/include
                    ../../../pjnath/include
                    ../../..
                   )

add_library( pjmedia-audiodev-lib STATIC 
		../../src/pjmedia-audiodev/audiodev.c
		../../src/pjmedia-audiodev/audiotest.c
		../../src/pjmedia-audiodev/errno.c
		../../src/pjmedia-audiodev/legacy_dev.c
		../../src/pjmedia-audiodev/null_dev.c
		../../src/pjmedia-audiodev/pa_dev.c
		../../src/pjmedia-audiodev/wmme_dev.c
		../../src/pjmedia-audiodev/alsa_dev.c
		../../src/pjmedia-audiodev/bb10_dev.c
		../../src/pjmedia-audiodev/bdimad_dev.c
		../../src/pjmedia-audiodev/android_jni_dev.c
		../../src/pjmedia-audiodev/opensl_dev.c
		)

find_library(log-lib log)


target_link_libraries(

		pjmedia-audiodev-lib
		log
		)

