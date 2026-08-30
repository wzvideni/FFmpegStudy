#ifndef FFMPEG_WRAPPER_H
#define FFMPEG_WRAPPER_H


#include "ffmpeg_basic.h"


class FFmpegWrapper {
public:
    static bool init();

    static string getVersion();

    static vector<string> getSupportedDecoders();

    static vector<string> getSupportedEncoders();

    static bool extractAudio(const string &inputPath, const string &outputPath);
};

#endif // FFMPEG_WRAPPER_H
