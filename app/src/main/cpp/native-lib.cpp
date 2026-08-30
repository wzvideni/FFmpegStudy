#include <jni.h>
#include "ffmpeg_wrapper.h"
#include "ffmpeg_utils.h"


extern "C" JNIEXPORT jstring JNICALL
Java_com_wzvideni_ffmpegstudy_FFmpegNative_ffmpegVersion(JNIEnv *env, jobject) {
    return env->NewStringUTF(FFmpegWrapper::getVersion().c_str());
}

extern "C" jboolean JNICALL
Java_com_wzvideni_ffmpegstudy_FFmpegNative_initFFmpeg(JNIEnv *, jobject) {
    return FFmpegWrapper::init() ? JNI_TRUE : JNI_FALSE;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_wzvideni_ffmpegstudy_FFmpegNative_getSupportedEncoders(JNIEnv *env, jobject /* this */) {
    auto decoderList = FFmpegWrapper::getSupportedEncoders();

    size_t vectorSize = decoderList.size();
    if (vectorSize > static_cast<size_t>(numeric_limits<jsize>::max())) {
        return nullptr;
    }

    jobjectArray decoderArray = env->NewObjectArray(static_cast<jsize>(vectorSize),
                                                    env->FindClass("java/lang/String"),
                                                    env->NewStringUTF(""));

    for (int j = 0; j < vectorSize; j++) {
        env->SetObjectArrayElement(decoderArray, j, env->NewStringUTF(decoderList[j].c_str()));
    }

    return decoderArray;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_wzvideni_ffmpegstudy_FFmpegNative_getSupportedDecoders(JNIEnv *env, jobject /* this */) {
    auto decoderList = FFmpegWrapper::getSupportedDecoders();

    size_t vectorSize = decoderList.size();
    if (vectorSize > static_cast<size_t>(numeric_limits<jsize>::max())) {
        return nullptr;
    }

    jobjectArray decoderArray = env->NewObjectArray(static_cast<jsize>(vectorSize),
                                                    env->FindClass("java/lang/String"),
                                                    env->NewStringUTF(""));

    for (int j = 0; j < vectorSize; j++) {
        env->SetObjectArrayElement(decoderArray, j, env->NewStringUTF(decoderList[j].c_str()));
    }

    return decoderArray;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_wzvideni_ffmpegstudy_FFmpegNative_extractAudio(JNIEnv *env, jobject, jstring inputPath_,
                                                        jstring outputPath_) {

    const char *inputPath = env->GetStringUTFChars(inputPath_, nullptr);
    const char *outputPath = env->GetStringUTFChars(outputPath_, nullptr);

    bool result = FFmpegWrapper::extractAudio(inputPath,
                                              FFmpegUtils::buildAudioOutputPath(inputPath,
                                                                                outputPath));

    env->ReleaseStringUTFChars(inputPath_, inputPath);
    env->ReleaseStringUTFChars(outputPath_, outputPath);

    return result ? JNI_TRUE : JNI_FALSE;
}