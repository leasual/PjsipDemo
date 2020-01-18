cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make libyuv-lib ################")

add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )

include_directories(
                    ../../../../jniLibs/armeabi/include
                    jni
                    /home/build/project/tools/android-ndk-r13b/sources/cxx-stl/gnu-libstdc++/4.9/include
                    /home/build/project/tools/android-ndk-r13b/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi/include
                    /home/build/project/output/openssl-build-output/libs/armeabi/include
                    /home/build/project/output/opus-build-output/libs/armeabi/include
                    .
                    ../../yuv/include
                    ../../../pjlib/include
                   )

add_library( libyuv-lib STATIC 
		../../yuv/source/compare.cc
		../../yuv/source/compare_common.cc
		../../yuv/source/compare_gcc.cc
		../../yuv/source/compare_neon64.cc
		../../yuv/source/compare_neon.cc
		../../yuv/source/compare_win.cc
		../../yuv/source/convert_argb.cc
		../../yuv/source/convert.cc
		../../yuv/source/convert_from_argb.cc
		../../yuv/source/convert_from.cc
		../../yuv/source/convert_jpeg.cc
		../../yuv/source/convert_to_argb.cc
		../../yuv/source/convert_to_i420.cc
		../../yuv/source/cpu_id.cc
		../../yuv/source/mjpeg_decoder.cc
		../../yuv/source/mjpeg_validate.cc
		../../yuv/source/planar_functions.cc
		../../yuv/source/rotate_any.cc
		../../yuv/source/rotate_argb.cc
		../../yuv/source/rotate.cc
		../../yuv/source/rotate_common.cc
		../../yuv/source/rotate_gcc.cc
		../../yuv/source/rotate_dspr2.cc
		../../yuv/source/rotate_neon64.cc
		../../yuv/source/rotate_neon.cc
		../../yuv/source/rotate_win.cc
		../../yuv/source/row_any.cc
		../../yuv/source/row_common.cc
		../../yuv/source/row_gcc.cc
		../../yuv/source/row_dspr2.cc
		../../yuv/source/row_neon64.cc
		../../yuv/source/row_neon.cc
		../../yuv/source/row_win.cc
		../../yuv/source/scale_any.cc
		../../yuv/source/scale_argb.cc
		../../yuv/source/scale.cc
		../../yuv/source/scale_common.cc
		../../yuv/source/scale_gcc.cc
		../../yuv/source/scale_dspr2.cc
		../../yuv/source/scale_neon64.cc
		../../yuv/source/scale_neon.cc
		../../yuv/source/scale_win.cc
		../../yuv/source/video_common.cc
		)

find_library(log-lib log)


target_link_libraries(

		libyuv-lib
		log
		)

