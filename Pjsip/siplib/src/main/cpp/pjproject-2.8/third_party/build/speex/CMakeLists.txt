cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make libspeex-lib ################")

add_definitions( -DHAVE_CONFIG_H )
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
                    ../../speex/include
                    ../../speex/libspeex
                    ../../../pjlib/include
                   )

add_library( libspeex-lib STATIC 
		../../speex/libspeex/bits.c
		../../speex/libspeex/cb_search.c
		../../speex/libspeex/exc_10_16_table.c
		../../speex/libspeex/exc_10_32_table.c
		../../speex/libspeex/exc_20_32_table.c
		../../speex/libspeex/exc_5_256_table.c
		../../speex/libspeex/exc_5_64_table.c
		../../speex/libspeex/exc_8_128_table.c
		../../speex/libspeex/fftwrap.c
		../../speex/libspeex/filterbank.c
		../../speex/libspeex/filters.c
		../../speex/libspeex/gain_table.c
		../../speex/libspeex/gain_table_lbr.c
		../../speex/libspeex/hexc_10_32_table.c
		../../speex/libspeex/hexc_table.c
		../../speex/libspeex/high_lsp_tables.c
		../../speex/libspeex/kiss_fft.c
		../../speex/libspeex/kiss_fftr.c
		../../speex/libspeex/lpc.c
		../../speex/libspeex/lsp.c
		../../speex/libspeex/lsp_tables_nb.c
		../../speex/libspeex/ltp.c
		../../speex/libspeex/mdf.c
		../../speex/libspeex/modes.c
		../../speex/libspeex/modes_wb.c
		../../speex/libspeex/nb_celp.c
		../../speex/libspeex/preprocess.c
		../../speex/libspeex/quant_lsp.c
		../../speex/libspeex/resample.c
		../../speex/libspeex/sb_celp.c
		../../speex/libspeex/smallft.c
		../../speex/libspeex/speex.c
		../../speex/libspeex/speex_callbacks.c
		../../speex/libspeex/speex_header.c
		../../speex/libspeex/stereo.c
		../../speex/libspeex/vbr.c
		../../speex/libspeex/vq.c
		../../speex/libspeex/window.c
		)

find_library(log-lib log)


target_link_libraries(

		libspeex-lib
		log
		)

