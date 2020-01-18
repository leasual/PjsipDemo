cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make libsrtp-lib ################")

add_definitions( -DHAVE_CONFIG_H )
add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )

include_directories(
                    .
                    ../../srtp/crypto/include
                    ../../srtp/include
                    ../../../pjlib/include
                    ../../../../jniLibs/armeabi/include
                    jni
                    /home/build/project/tools/android-ndk-r13b/sources/cxx-stl/gnu-libstdc++/4.9/include
                    /home/build/project/tools/android-ndk-r13b/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi/include
                    /home/build/project/output/openssl-build-output/libs/armeabi/include
                    /home/build/project/output/opus-build-output/libs/armeabi/include
                   )

add_library( libsrtp-lib STATIC 
		../../srtp/crypto/cipher/cipher.c
		../../srtp/crypto/cipher/null_cipher.c
		../../srtp/crypto/cipher/aes.c
		../../srtp/crypto/cipher/aes_icm.c
		../../srtp/crypto/hash/null_auth.c
		../../srtp/crypto/hash/auth.c
		../../srtp/crypto/hash/sha1.c
		../../srtp/crypto/hash/hmac.c
		../../srtp/crypto/math/datatypes.c
		../../srtp/crypto/math/stat.c
		../../srtp/crypto/kernel/crypto_kernel.c
		../../srtp/crypto/kernel/alloc.c
		../../srtp/crypto/kernel/key.c
		../../srtp/pjlib/srtp_err.c
		../../srtp/crypto/replay/rdb.c
		../../srtp/crypto/replay/rdbx.c
		../../srtp/crypto/replay/ut_sim.c
		../../srtp/srtp/srtp.c
		../../srtp/srtp/ekt.c
		)

find_library(log-lib log)


target_link_libraries(

		libsrtp-lib
		log
		)

