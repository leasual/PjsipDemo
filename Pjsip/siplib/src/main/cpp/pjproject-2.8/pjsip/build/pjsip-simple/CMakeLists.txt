cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjsip-simple-lib ################")

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
                   )

add_library( pjsip-simple-lib STATIC 
		../../src/pjsip-simple/errno.c
		../../src/pjsip-simple/evsub.c
		../../src/pjsip-simple/evsub_msg.c
		../../src/pjsip-simple/iscomposing.c
		../../src/pjsip-simple/mwi.c
		../../src/pjsip-simple/pidf.c
		../../src/pjsip-simple/presence.c
		../../src/pjsip-simple/presence_body.c
		../../src/pjsip-simple/publishc.c
		../../src/pjsip-simple/rpid.c
		../../src/pjsip-simple/xpidf.c
		)

find_library(log-lib log)


target_link_libraries(

		pjsip-simple-lib
		log
		)

