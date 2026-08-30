package com.wzvideni.ffmpegstudy

class FFmpegNative {
    companion object {
        init {
            System.loadLibrary("avutil")
            System.loadLibrary("avcodec")
            System.loadLibrary("avfilter")
            System.loadLibrary("avformat")
            System.loadLibrary("swscale")
            System.loadLibrary("swresample")
            System.loadLibrary("avdevice")
            System.loadLibrary("native-lib")
        }
    }

    external fun ffmpegVersion(): String

    external fun initFFmpeg(): Boolean

    external fun getSupportedEncoders(): Array<String>

    external fun getSupportedDecoders(): Array<String>

    external fun extractAudio(inputPath: String, outputPath: String = ""): Boolean
}
