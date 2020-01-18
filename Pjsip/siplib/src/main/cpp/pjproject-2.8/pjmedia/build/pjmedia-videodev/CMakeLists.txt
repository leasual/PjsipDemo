cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjmedia-videodev-lib ################")

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
                    ../../../../jniLibs/armeabi/include
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

add_library( pjmedia-videodev-lib STATIC 
		../../src/pjmedia-videodev/android_dev.c
		../../src/pjmedia-videodev/android_opengl.c
		../../src/pjmedia-videodev/sdl_dev.c
		../../src/pjmedia-videodev/videodev.c
		../../src/pjmedia-videodev/errno.c
		../../src/pjmedia-videodev/avi_dev.c
		../../src/pjmedia-videodev/ffmpeg_dev.c
		../../src/pjmedia-videodev/colorbar_dev.c
		../../src/pjmedia-videodev/v4l2_dev.c
		../../src/pjmedia-videodev/opengl_dev.c
		../../src/pjmedia-videodev/util.c
		)

find_library(log-lib log)


target_link_libraries(

		pjmedia-videodev-lib
		log
		)

