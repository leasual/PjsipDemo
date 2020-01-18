cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make libresample-lib ################")

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
                    ../../resample/include
                    ../../../pjlib/include
                   )

add_library( libresample-lib STATIC 
		../../resample/src/resamplesubs.c
		)

find_library(log-lib log)


target_link_libraries(

		libresample-lib
		log
		)

