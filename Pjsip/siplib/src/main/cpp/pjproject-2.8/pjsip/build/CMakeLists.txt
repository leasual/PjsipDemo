cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjsip-lib ################")

add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )

include_directories(
					../../auto_config/${ANDROID_ABI}
					../../../openh264/include
                    ../../../../jniLibs/armeabi/include
                    jni

                    ../include
                    ../../pjlib/include
                    ../../pjlib-util/include
                    ../../pjnath/include
                    ../../pjmedia/include
                   )

add_library( pjsip-lib STATIC 
		../src/pjsip/sip_config.c
		../src/pjsip/sip_multipart.c
		../src/pjsip/sip_errno.c
		../src/pjsip/sip_msg.c
		../src/pjsip/sip_parser.c
		../src/pjsip/sip_tel_uri.c
		../src/pjsip/sip_uri.c
		../src/pjsip/sip_endpoint.c
		../src/pjsip/sip_util.c
		../src/pjsip/sip_util_proxy.c
		../src/pjsip/sip_resolve.c
		../src/pjsip/sip_transport.c
		../src/pjsip/sip_transport_loop.c
		../src/pjsip/sip_transport_udp.c
		../src/pjsip/sip_transport_tcp.c
		../src/pjsip/sip_transport_tls.c
		../src/pjsip/sip_auth_aka.c
		../src/pjsip/sip_auth_client.c
		../src/pjsip/sip_auth_msg.c
		../src/pjsip/sip_auth_parser.c
		../src/pjsip/sip_auth_server.c
		../src/pjsip/sip_transaction.c
		../src/pjsip/sip_util_statefull.c
		../src/pjsip/sip_dialog.c
		../src/pjsip/sip_ua_layer.c
		)

find_library(log-lib log)


target_link_libraries(

		pjsip-lib
		log
		)

