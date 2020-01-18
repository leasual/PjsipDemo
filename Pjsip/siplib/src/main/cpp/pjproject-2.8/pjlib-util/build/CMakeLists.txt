cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjlib-util-lib ################")

add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )

include_directories(
					../../auto_config/${ANDROID_ABI}
					../../../openh264/include

                    jni

                    ../include
                    ../../pjlib/include
                   )

add_library( pjlib-util-lib STATIC 
		../src/pjlib-util/base64.c
		../src/pjlib-util/cli.c
		../src/pjlib-util/cli_console.c
		../src/pjlib-util/cli_telnet.c
		../src/pjlib-util/crc32.c
		../src/pjlib-util/errno.c
		../src/pjlib-util/dns.c
		../src/pjlib-util/dns_dump.c
		../src/pjlib-util/dns_server.c
		../src/pjlib-util/getopt.c
		../src/pjlib-util/hmac_md5.c
		../src/pjlib-util/hmac_sha1.c
		../src/pjlib-util/http_client.c
		../src/pjlib-util/json.c
		../src/pjlib-util/md5.c
		../src/pjlib-util/pcap.c
		../src/pjlib-util/resolver.c
		../src/pjlib-util/scanner.c
		../src/pjlib-util/sha1.c
		../src/pjlib-util/srv_resolver.c
		../src/pjlib-util/string.c
		../src/pjlib-util/stun_simple.c
		../src/pjlib-util/stun_simple_client.c
		../src/pjlib-util/xml.c
		)

find_library(log-lib log)


target_link_libraries(

		pjlib-util-lib
		log
		)

