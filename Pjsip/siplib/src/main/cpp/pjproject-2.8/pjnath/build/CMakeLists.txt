cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjnath-lib ################")

add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )

include_directories(
					../../auto_config/${ANDROID_ABI}
					../../../../openh264/include

                    jni

                    ../include
                    ../../pjlib/include
                    ../../pjlib-util/include
                   )

add_library( pjnath-lib STATIC 
		../src/pjnath/errno.c
		../src/pjnath/ice_session.c
		../src/pjnath/ice_strans.c
		../src/pjnath/nat_detect.c
		../src/pjnath/stun_auth.c
		../src/pjnath/stun_msg.c
		../src/pjnath/stun_msg_dump.c
		../src/pjnath/stun_session.c
		../src/pjnath/stun_sock.c
		../src/pjnath/stun_transaction.c
		../src/pjnath/turn_session.c
		../src/pjnath/turn_sock.c
		)

find_library(log-lib log)


target_link_libraries(

		pjnath-lib
		log
		)

