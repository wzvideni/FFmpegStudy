# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).


project("native-lib")

# 添加 FFmpeg 头文件路径
include_directories(include)

# 定义 FFmpeg 库名称（需与 so 文件名匹配）
set(FFMPEG_LIBS avutil avcodec avdevice avfilter avformat swresample swscale)

set(JNI_LIBS_DIR ${PROJECT_ROOT_DIR}/app/src/main/jniLibs)

# 动态链接 FFmpeg 库
add_library(lib_avutil SHARED IMPORTED)
add_library(lib_avcodec SHARED IMPORTED)
add_library(lib_avdevice SHARED IMPORTED)
add_library(lib_avfilter SHARED IMPORTED)
add_library(lib_avformat SHARED IMPORTED)
add_library(lib_swresample SHARED IMPORTED)
add_library(lib_swscale SHARED IMPORTED)
set_target_properties(lib_avutil PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavutil.so)
set_target_properties(lib_avcodec PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavcodec.so)
set_target_properties(lib_avdevice PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavdevice.so)
set_target_properties(lib_avfilter PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavfilter.so)
set_target_properties(lib_avformat PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavformat.so)
set_target_properties(lib_swresample PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/${ANDROID_ABI}/libswresample.so)
set_target_properties(lib_swscale PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/${ANDROID_ABI}/libswscale.so)

# 添加宏定义
add_definitions(-D__STDC_CONSTANT_MACROS)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.

# 编译 JNI 代码
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        ffmpeg_utils.cpp
        ffmpeg_wrapper.cpp
)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.

# 链接 FFmpeg 库到 JNI 代码
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log
        lib_avutil
        lib_avcodec
        lib_avdevice
        lib_avfilter
        lib_avformat
        lib_swresample
        lib_swscale
)