# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

add_definitions( -D_ARM_ )
add_definitions( -DNDEBUG )
add_definitions( -DANDROID )
add_definitions( -DPJ_IS_BIG_ENDIAN=0 )
add_definitions( -DPJ_IS_LITTLE_ENDIAN=1 )

include_directories(
        src/main/cpp/pjproject-2.8/auto_config/${ANDROID_ABI}
        src/main/cpp/pjproject-2.8/pjsip/include
        src/main/cpp/pjproject-2.8/pjmedia/include
        src/main/cpp/pjproject-2.8/pjlib/include
        src/main/cpp/pjproject-2.8/pjlib-util/include
        src/main/cpp/pjproject-2.8/pjnath/include

)
LINK_DIRECTORIES(libs/${ANDROID_ABI}/)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        pjsua2

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp
        src/main/cpp/pjsua2_wrap.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        pjsua2

        pjsua2-lib

        stdc++

        pjsua-lib
        pjsip-ua-lib
        pjsip-simple-lib
        pjsip-lib

        pjmedia-codec-lib
        pjmedia-lib

        pjmedia-videodev-lib
        pjmedia-audiodev-lib

        pjmedia-lib

        pjnath-lib
        pjlib-util-lib

        libsrtp.a
        libresample.a
        libgsmcodec.a
        libspeex.a
        libilbccodec.a
        libg7221codec.a
        libyuv.a
        libwebrtc.a
        pjlib-lib
        libopenh264.so




        OpenSLES
        GLESv2
        EGL
        android
        mediandk
        OpenMAXAL




        # Links the target library to the log library
        # included in the NDK.




        ${log-lib})


add_subdirectory( src/main/cpp/pjproject-2.8/pjlib/build )
add_subdirectory( src/main/cpp/pjproject-2.8/pjlib-util/build )
add_subdirectory( src/main/cpp/pjproject-2.8/pjnath/build )

add_subdirectory( src/main/cpp/pjproject-2.8/pjmedia/build/pjmedia-codec )
add_subdirectory( src/main/cpp/pjproject-2.8/pjmedia/build/pjmedia-videodev )
add_subdirectory( src/main/cpp/pjproject-2.8/pjmedia/build/pjmedia-audiodev )
add_subdirectory( src/main/cpp/pjproject-2.8/pjmedia/build )

add_subdirectory( src/main/cpp/pjproject-2.8/pjsip/build )

add_subdirectory( src/main/cpp/pjproject-2.8/pjsip/build/pjsip-ua )
add_subdirectory( src/main/cpp/pjproject-2.8/pjsip/build/pjsip-simple )
add_subdirectory( src/main/cpp/pjproject-2.8/pjsip-apps/src/pjsystest )
add_subdirectory( src/main/cpp/pjproject-2.8/pjsip/build/pjsua-lib )
add_subdirectory( src/main/cpp/pjproject-2.8/pjsip/build/pjsua2-lib )
add_subdirectory( src/main/cpp/pjproject-2.8/pjsip-apps/src/samples )
add_subdirectory( src/main/cpp/pjproject-2.8/third_party/build/webrtc )
#add_subdirectory( src/main/cpp/pjproject-2.8/pjmedia/src/test )
