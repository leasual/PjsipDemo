cmake_minimum_required(VERSION 3.4.1)

message(WARNING "########make libpjsua2 ################")




LINK_DIRECTORIES(D:/work/andriod_workspace/MyApplication2/app/src/main/jniLibs/${ANDROID_ABI}/)

add_library( native-lib SHARED
		 pjsip-apps/src/swig/java/output/pjsua2_wrap.cpp
		)



find_library(log-lib log)


target_link_libraries(
        native-lib

		pjsua2-lib
		pjsua-lib
		pjsip-simple-lib
		pjsip-ua-lib

		pjsip-lib

		pjmedia-lib
		pjmedia-audiodev-lib
		pjmedia-videodev-lib
		pjmedia-codec-lib

		libwebrtc.a
		libyuv.a
		libilbccodec.a
		libspeex.a
		libsrtp.a
		libresample.a

		pjnath-lib
		pjlib-util-lib
		pjlib-lib

		${log-lib}

		)



add_subdirectory( pjlib/build )
add_subdirectory( pjlib-util/build )
add_subdirectory( pjnath/build )

add_subdirectory( pjmedia/build/pjmedia-codec )
add_subdirectory( pjmedia/build/pjmedia-videodev )
add_subdirectory( pjmedia/build/pjmedia-audiodev )
add_subdirectory( pjmedia/build )

add_subdirectory( pjsip/build )

add_subdirectory( pjsip/build/pjsip-ua )
add_subdirectory( pjsip/build/pjsip-simple )
add_subdirectory( pjsip/build/pjsua-lib )
add_subdirectory( pjsip/build/pjsua2-lib )

