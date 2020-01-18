cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make pjlib-lib ################")

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
                   )

add_library( pjlib-lib STATIC 
		../src/pj/ioqueue_select.c
		../src/pj/file_access_unistd.c
		../src/pj/file_io_ansi.c
		../src/pj/os_core_unix.c
		../src/pj/os_error_unix.c
		../src/pj/os_time_unix.c
		../src/pj/os_timestamp_posix.c
		../src/pj/sock_qos_bsd.c
		../src/pj/guid_android.c
		../src/pj/addr_resolv_sock.c
		../src/pj/log_writer_stdout.c
		../src/pj/os_timestamp_common.c
		../src/pj/pool_policy_malloc.c
		../src/pj/sock_bsd.c
		../src/pj/sock_select.c
		../src/pj/activesock.c
		../src/pj/array.c
		../src/pj/config.c
		../src/pj/ctype.c
		../src/pj/errno.c
		../src/pj/except.c
		../src/pj/fifobuf.c
		../src/pj/guid.c
		../src/pj/hash.c
		../src/pj/ip_helper_generic.c
		../src/pj/list.c
		../src/pj/lock.c
		../src/pj/log.c
		../src/pj/os_time_common.c
		../src/pj/os_info.c
		../src/pj/pool.c
		../src/pj/pool_buf.c
		../src/pj/pool_caching.c
		../src/pj/pool_dbg.c
		../src/pj/rand.c
		../src/pj/rbtree.c
		../src/pj/sock_common.c
		../src/pj/sock_qos_common.c
		../src/pj/ssl_sock_common.c
		../src/pj/ssl_sock_ossl.c
		../src/pj/ssl_sock_gtls.c
		../src/pj/ssl_sock_dump.c
		../src/pj/string.c
		../src/pj/timer.c
		../src/pj/types.c
		)

find_library(log-lib log)


target_link_libraries(

		pjlib-lib
		log
		)

