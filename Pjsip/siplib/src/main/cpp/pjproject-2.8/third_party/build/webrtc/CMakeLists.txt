cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make libwebrtc-lib ################")

add_definitions( -DPJ_AUTOCONF=1 )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )
add_definitions( -DWEBRTC_POSIX=1 )
add_definitions( -DWEBRTC_HAS_NEON )

include_directories(
                    ../../../../jniLibs/armeabi/include
                    jni
                    /home/build/project/tools/android-ndk-r13b/sources/cxx-stl/gnu-libstdc++/4.9/include
                    /home/build/project/tools/android-ndk-r13b/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi/include
                    /home/build/project/output/openssl-build-output/libs/armeabi/include
                    /home/build/project/output/opus-build-output/libs/armeabi/include
                    .
                    ../../webrtc/src
                    ../../../pjlib/include
                   )

add_library( libwebrtc-lib STATIC 
		../../webrtc/src/webrtc//modules/audio_processing/aec/aec_core.c
		../../webrtc/src/webrtc//modules/audio_processing/aec/aec_rdft.c
		../../webrtc/src/webrtc//modules/audio_processing/aec/aec_resampler.c
		../../webrtc/src/webrtc//modules/audio_processing/aec/echo_cancellation.c
		../../webrtc/src/webrtc//modules/audio_processing/aecm/aecm_core.c
		../../webrtc/src/webrtc//modules/audio_processing/aecm/echo_control_mobile.c
		../../webrtc/src/webrtc//modules/audio_processing/ns/noise_suppression.c
		../../webrtc/src/webrtc//modules/audio_processing/ns/noise_suppression_x.c
		../../webrtc/src/webrtc//modules/audio_processing/ns/ns_core.c
		../../webrtc/src/webrtc//modules/audio_processing/ns/nsx_core.c
		../../webrtc/src/webrtc//modules/audio_processing/utility/delay_estimator_wrapper.c
		../../webrtc/src/webrtc//modules/audio_processing/utility/delay_estimator.c
		../../webrtc/src/webrtc//common_audio/fft4g.c
		../../webrtc/src/webrtc//common_audio/ring_buffer.c
		../../webrtc/src/webrtc//common_audio/signal_processing/complex_bit_reverse.c
		../../webrtc/src/webrtc//common_audio/signal_processing/complex_fft.c
		../../webrtc/src/webrtc//common_audio/signal_processing/copy_set_operations.c
		../../webrtc/src/webrtc//common_audio/signal_processing/cross_correlation.c
		../../webrtc/src/webrtc//common_audio/signal_processing/division_operations.c
		../../webrtc/src/webrtc//common_audio/signal_processing/downsample_fast.c
		../../webrtc/src/webrtc//common_audio/signal_processing/energy.c
		../../webrtc/src/webrtc//common_audio/signal_processing/get_scaling_square.c
		../../webrtc/src/webrtc//common_audio/signal_processing/min_max_operations.c
		../../webrtc/src/webrtc//common_audio/signal_processing/randomization_functions.c
		../../webrtc/src/webrtc//common_audio/signal_processing/real_fft.c
		../../webrtc/src/webrtc//common_audio/signal_processing/spl_init.c
		../../webrtc/src/webrtc//common_audio/signal_processing/spl_sqrt.c
		../../webrtc/src/webrtc//common_audio/signal_processing/spl_sqrt_floor.c
		../../webrtc/src/webrtc//common_audio/signal_processing/vector_scaling_operations.c
		../../webrtc/src/webrtc//modules/audio_processing/aec/aec_core_neon.c
		../../webrtc/src/webrtc//modules/audio_processing/aec/aec_rdft_neon.c
		../../webrtc/src/webrtc//modules/audio_processing/aecm/aecm_core_c.c
		../../webrtc/src/webrtc//modules/audio_processing/aecm/aecm_core_neon.c
		../../webrtc/src/webrtc//modules/audio_processing/ns/nsx_core_c.c
		../../webrtc/src/webrtc//modules/audio_processing/ns/nsx_core_neon.c
		../../webrtc/src/webrtc//common_audio/signal_processing/cross_correlation_neon.c
		../../webrtc/src/webrtc//common_audio/signal_processing/downsample_fast_neon.c
		../../webrtc/src/webrtc//common_audio/signal_processing/min_max_operations_neon.c
		)

find_library(log-lib log)


target_link_libraries(

		libwebrtc-lib
		log
		)

