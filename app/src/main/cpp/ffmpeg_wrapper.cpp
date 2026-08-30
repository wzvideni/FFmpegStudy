#include "ffmpeg_wrapper.h"
#include "ffmpeg_utils.h"

#undef LOG_TAG
#define LOG_TAG "FFmpegWrapper"

string FFmpegWrapper::getVersion() {
    return av_version_info();
}


bool FFmpegWrapper::init() {
    const char *version = av_version_info();
    avformat_network_init();
    LOGD("FFmpeg initialized with version: %s", version);
    return true;
}


vector<string> FFmpegWrapper::getSupportedEncoders() {
    vector<string> decoderList;
    const AVCodec *codec = nullptr;
    void *i = nullptr;

    while ((codec = av_codec_iterate(&i))) {
        if (av_codec_is_encoder(codec)) {
            decoderList.emplace_back(codec->name);
        }
    }

    return decoderList;
}


vector<string> FFmpegWrapper::getSupportedDecoders() {
    vector<string> decoderList;
    const AVCodec *codec = nullptr;
    void *i = nullptr;

    while ((codec = av_codec_iterate(&i))) {
        if (av_codec_is_decoder(codec)) {
            decoderList.emplace_back(codec->name);
        }
    }

    return decoderList;
}


// 从视频内提取音频
bool FFmpegWrapper::extractAudio(const string &inputPath, const string &outputPath) {

    AVFormatContext *inputCtx = nullptr;
    AVFormatContext *outputCtx = nullptr;

    bool success = false;

    do {
        AVStream *inAudioStream = nullptr;
        AVStream *outAudioStream = nullptr;

        // 打开输入文件
        if (avformat_open_input(&inputCtx, inputPath.c_str(), nullptr, nullptr) < 0) {
            LOGD("Failed to open input file: %s", inputPath.c_str());
            break;
        }
        // 找到音频流
        if (avformat_find_stream_info(inputCtx, nullptr) < 0) {
            LOGD("Failed to find stream info for input file: %s", inputPath.c_str());
            break;
        }

        int audioStreamIndex = av_find_best_stream(inputCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr,
                                                   0);
        if (audioStreamIndex < 0) {
            LOGD("No audio stream found in input file: %s", inputPath.c_str());
            break;
        } else {
            inAudioStream = inputCtx->streams[audioStreamIndex];
        }

        // 创建输出上下文
        if (avformat_alloc_output_context2(&outputCtx, nullptr, nullptr, outputPath.c_str()) < 0) {
            LOGD("Failed to allocate output context");
            break;
        }

        // 复制音频流
        outAudioStream = avformat_new_stream(outputCtx, nullptr);
        if (!outAudioStream) {
            LOGD("Failed to create output stream");
            break;
        }

        if (avcodec_parameters_copy(outAudioStream->codecpar, inAudioStream->codecpar) < 0) {
            LOGD("Failed to copy codec parameters");
            break;
        }

        outAudioStream->codecpar->codec_tag = 0;
        outAudioStream->time_base = inAudioStream->time_base;

        // 打开输出文件
        if (!(outputCtx->oformat->flags & AVFMT_NOFILE)) {
            if (avio_open(&outputCtx->pb, outputPath.c_str(), AVIO_FLAG_WRITE) < 0) {
                LOGD("Could not open output file: %s", outputPath.c_str());
                break;
            }
        }

        // 写头部
        if (avformat_write_header(outputCtx, nullptr) < 0) {
            LOGD("Failed to write header for output file: %s", outputPath.c_str());
            break;
        }

        // 复制音频数据
        AVPacket packet;
        while (av_read_frame(inputCtx, &packet) >= 0) {
            if (packet.stream_index == audioStreamIndex) {
                packet.stream_index = outAudioStream->index;
                if (av_interleaved_write_frame(outputCtx, &packet) < 0) {
                    LOGD("Failed to write frame to output file: %s", outputPath.c_str());
                    av_packet_unref(&packet);
                    break;
                }
            }
            av_packet_unref(&packet);
        }

        // 写尾部
        if (av_write_trailer(outputCtx) < 0) {
            LOGD("Failed to write trailer for output file: %s", outputPath.c_str());
            break;
        }
        success = true;
    } while (false); // 利用 do{}while(false) 简化早退逻辑

    // 清理资源（统一释放）
    if (inputCtx) avformat_close_input(&inputCtx);
    if (outputCtx) {
        if (!(outputCtx->oformat->flags & AVFMT_NOFILE) && outputCtx->pb) {
            avio_closep(&outputCtx->pb);
        }
        avformat_free_context(outputCtx);
    }

    return success;
}
