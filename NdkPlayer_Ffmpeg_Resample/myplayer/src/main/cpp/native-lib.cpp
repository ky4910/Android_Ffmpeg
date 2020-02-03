#include <jni.h>
#include <string>
#include <android/log.h>

#include "KyCallJava.h"
#include "KyFfmpeg.h"

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/avutil.h>
}

#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"ky4910",FORMAT,##__VA_ARGS__);


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_kyndkplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myplayer_Demo_stringFromJNI(JNIEnv *env, jobject thiz) {

    // TODO: implement stringFromJNI()
    std::string hello = "Welcome to ffmpeg world!";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myplayer_Demo_testFfmpeg(JNIEnv *env, jobject thiz) {

    // TODO: implement testFfmpeg()
    av_register_all();
    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL)
    {
        switch (c_temp->type)
        {
            case AVMEDIA_TYPE_VIDEO:
                LOGI("[Video]:%s", c_temp->name);
                break;
            case AVMEDIA_TYPE_AUDIO:
                LOGI("[Audio]:%s", c_temp->name);
                break;
            default:
                LOGI("[Other]:%s", c_temp->name);
                break;
        }
        c_temp = c_temp->next;
    }
    std::string hello = "hello from jni";
    return env->NewStringUTF(hello.c_str());
}

JavaVM *javaVm = NULL;
KyCallJava *callJava = NULL;
KyFfmpeg *kyFfmpeg = NULL;
KyPlayStatus *kyPlayStatus = NULL;

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVm = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }
    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myplayer_player_KyPlayer_n_1prepared(JNIEnv *env, jobject thiz, jstring source) {

    LOGI("this is log from native-lib!");

    const char *mSrc = env->GetStringUTFChars(source, 0);

    // TODO: implement n_prepared()
    if (kyFfmpeg == NULL) {
        if (callJava == NULL) {
            callJava = new KyCallJava(javaVm, env, &thiz);
        }
        kyPlayStatus = new KyPlayStatus();
        kyFfmpeg = new KyFfmpeg(kyPlayStatus, callJava, mSrc);
        kyFfmpeg->prepared();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myplayer_player_KyPlayer_n_1start(JNIEnv *env, jobject thiz) {

    // TODO: implement n_start()
    if (kyFfmpeg != NULL) {
        kyFfmpeg->start();
    }

}