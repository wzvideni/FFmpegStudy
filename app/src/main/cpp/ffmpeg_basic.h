
#ifndef FFMPEG_H
#define FFMPEG_H

extern "C" {
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavdevice/avdevice.h>
}


#include <android/log.h>
#include <vector>
#include <string>
#include <map>

using namespace std;


#define LOG_TAG "FFmpegNative"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)


#endif // FFMPEG_H
