#ifndef FFMPEG_UTILS_H
#define FFMPEG_UTILS_H


#include "ffmpeg_basic.h"


class FFmpegUtils {
public:
    static string getAudioExtensionFromCodecId(AVCodecID codecId);

    static string buildAudioOutputPath(const string &inputPath, const string &outputPath);
};

#endif // FFMPEG_UTILS_H