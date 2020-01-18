cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjsua2-lib ################")

add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )
add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )

include_directories(
					../../../auto_config/${ANDROID_ABI}
					../../../../openh264/include
                    ../../../../jniLibs/armeabi/include
                    ../jni

                    ../../include
                    ../../../pjlib/include
                    ../../../pjlib-util/include
                    ../../../pjnath/include
                    ../../../pjmedia/include
                    ../../../../jniLibs/armeabi/include
                    ../jni

                   )

add_library( pjsua2-lib STATIC 
		../../src/pjsua2/account.cpp
		../../src/pjsua2/endpoint.cpp
		../../src/pjsua2/json.cpp
		../../src/pjsua2/persistent.cpp
		../../src/pjsua2/types.cpp
		../../src/pjsua2/siptypes.cpp
		../../src/pjsua2/call.cpp
		../../src/pjsua2/presence.cpp
		../../src/pjsua2/media.cpp
		)

find_library(log-lib log)


target_link_libraries(

		pjsua2-lib
		log
		)

