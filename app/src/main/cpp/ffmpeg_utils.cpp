#include "ffmpeg_utils.h"

#undef LOG_TAG
#define LOG_TAG "FFmpegUtils"

// 根据音频编码器ID获取音频的扩展名
string FFmpegUtils::getAudioExtensionFromCodecId(AVCodecID codecId) {
    static const map<AVCodecID, string> codecExtensions = {
            {AV_CODEC_ID_AAC,       "aac"}, // raw AAC
            {AV_CODEC_ID_MP3,       "mp3"},
            {AV_CODEC_ID_FLAC,      "flac"},
            {AV_CODEC_ID_ALAC,      "m4a"}, // ALAC in M4A container
            {AV_CODEC_ID_OPUS,      "opus"},
            {AV_CODEC_ID_VORBIS,    "ogg"}, // vorbis often in .ogg
            {AV_CODEC_ID_WMAV2,     "wma"},
            {AV_CODEC_ID_AC3,       "ac3"},
            {AV_CODEC_ID_EAC3,      "eac3"},
            {AV_CODEC_ID_PCM_S16LE, "wav"},
            {AV_CODEC_ID_PCM_S24LE, "wav"},
            {AV_CODEC_ID_PCM_S32LE, "wav"},
            {AV_CODEC_ID_PCM_F32LE, "wav"},
            {AV_CODEC_ID_PCM_F64LE, "wav"},
            {AV_CODEC_ID_AMR_NB,    "amr"},
            {AV_CODEC_ID_AMR_WB,    "awb"},
            {AV_CODEC_ID_ATRAC3,    "oma"}, // Sony OpenMG
            {AV_CODEC_ID_TRUEHD,    "thd"}, // Dolby TrueHD
            {AV_CODEC_ID_DTS,       "dts"},
            {AV_CODEC_ID_APE,       "ape"},
            {AV_CODEC_ID_TAK,       "tak"},
            {AV_CODEC_ID_MP2,       "mp2"},
            {AV_CODEC_ID_COOK,      "ra"}, // RealAudio
            {AV_CODEC_ID_QDM2,      "mov"}, // QuickTime audio
    };

    auto it = codecExtensions.find(codecId);
    if (it != codecExtensions.end()) {
        return it->second;
    }
    return "bin"; // 默认拓展名
}

// 根据输入视频的音频格式构建音频的输出路径
string
FFmpegUtils::buildAudioOutputPath(const string &inputPath, const string &outputPath) {
    AVFormatContext *inputCtx = nullptr;
    if (avformat_open_input(&inputCtx, inputPath.c_str(), nullptr, nullptr) < 0) {
        LOGD("Failed to open input file: %s", inputPath.c_str());
        return "";
    }

    if (avformat_find_stream_info(inputCtx, nullptr) < 0) {
        LOGD("Failed to find stream info for input file: %s", inputPath.c_str());
        avformat_close_input(&inputCtx);
        return "";
    }

    int audioStreamIndex = av_find_best_stream(inputCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (audioStreamIndex < 0) {
        LOGD("No audio stream found in input file: %s", inputPath.c_str());
        avformat_close_input(&inputCtx);
        return "";
    }

    AVStream *audioStream = inputCtx->streams[audioStreamIndex];
    AVCodecParameters *codecPar = audioStream->codecpar;
    AVCodecID codecId = codecPar->codec_id;

    string ext = getAudioExtensionFromCodecId(codecId);

    avformat_close_input(&inputCtx);

    std::string basePath = outputPath.empty() ? inputPath : outputPath;
    size_t lastDot = basePath.find_last_of('.');
    if (lastDot != std::string::npos) {
        basePath = basePath.substr(0, lastDot);
    }
    return basePath + "." + ext;
}


