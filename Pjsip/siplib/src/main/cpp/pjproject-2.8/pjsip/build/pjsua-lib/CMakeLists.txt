cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjsua-lib ################")

add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )
add_definitions( -DPJMEDIA_HAS_OPENH264_CODEC=1 )
add_definitions( -DPJMEDIA_HAS_OPENH264_CODEC=1 )

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
                    ../../../../jniLibs/armeabi/include
                   )

add_library( pjsua-lib STATIC 
		../../src/pjsua-lib/pjsua_acc.c
		../../src/pjsua-lib/pjsua_call.c
		../../src/pjsua-lib/pjsua_core.c
		../../src/pjsua-lib/pjsua_im.c
		../../src/pjsua-lib/pjsua_media.c
		../../src/pjsua-lib/pjsua_pres.c
		../../src/pjsua-lib/pjsua_dump.c
		../../src/pjsua-lib/pjsua_aud.c
		../../src/pjsua-lib/pjsua_vid.c
		)

find_library(log-lib log)


target_link_libraries(

		pjsua-lib
		log
		)

