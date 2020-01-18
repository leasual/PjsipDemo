cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make libilbccodec-lib ################")

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
                    ../../ilbc
                    ../../../pjlib/include
                   )

add_library( libilbccodec-lib STATIC 
		../../ilbc/FrameClassify.c
		../../ilbc/LPCdecode.c
		../../ilbc/LPCencode.c
		../../ilbc/StateConstructW.c
		../../ilbc/StateSearchW.c
		../../ilbc/anaFilter.c
		../../ilbc/constants.c
		../../ilbc/createCB.c
		../../ilbc/doCPLC.c
		../../ilbc/enhancer.c
		../../ilbc/filter.c
		../../ilbc/gainquant.c
		../../ilbc/getCBvec.c
		../../ilbc/helpfun.c
		../../ilbc/hpInput.c
		../../ilbc/hpOutput.c
		../../ilbc/iCBConstruct.c
		../../ilbc/iCBSearch.c
		../../ilbc/iLBC_decode.c
		../../ilbc/iLBC_encode.c
		../../ilbc/lsf.c
		../../ilbc/packing.c
		../../ilbc/syntFilter.c
		)

find_library(log-lib log)


target_link_libraries(

		libilbccodec-lib
		log
		)

