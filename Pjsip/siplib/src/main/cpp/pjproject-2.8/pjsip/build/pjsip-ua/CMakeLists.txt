cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjsip-ua-lib ################")

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

add_library( pjsip-ua-lib STATIC 
		../../src/pjsip-ua/sip_inv.c
		../../src/pjsip-ua/sip_reg.c
		../../src/pjsip-ua/sip_replaces.c
		../../src/pjsip-ua/sip_xfer.c
		../../src/pjsip-ua/sip_100rel.c
		../../src/pjsip-ua/sip_timer.c
		)

find_library(log-lib log)


target_link_libraries(

		pjsip-ua-lib
		log
		)

