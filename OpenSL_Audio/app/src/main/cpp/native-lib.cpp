#include <jni.h>
#include <string>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "android/log.h"

#include <android/log.h>
#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"ky4910",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"ky4910",FORMAT,##__VA_ARGS__);


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_kyopenslpcm_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

// 引擎接口
SLObjectItf engineObject = NULL;
SLEngineItf engineEngine = NULL;

// 混音器
SLObjectItf outputMixObject = NULL;
SLEnvironmentalReverbItf outputMixEnvReverb = NULL;
SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

// pcm
SLObjectItf  pcmPlayerObject = NULL;
SLPlayItf pcmPlayerPlay = NULL;

// 缓冲器队列接口
SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;

#include <errno.h>

FILE *pcmFile;
void *buffer;
uint8_t *out_buffer;

int getPcmData(void **pcm) {

    int size = 0;
    while(!feof(pcmFile)) {
        size = fread(out_buffer, 1, 44100*2*2, pcmFile);
        if (out_buffer == NULL) {
            LOGE("%s", "read end!");
            break;
        } else {
            LOGE("%s", "reading...");
        }
        *pcm = out_buffer;
        break;
    }
    return size;
}

void pcmBufferCallback(SLAndroidSimpleBufferQueueItf bf, void *context) {
    int size = getPcmData(&buffer);
    if (buffer != NULL) {
        (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, size);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_kyopenslpcm_MainActivity_n_1playpcm(JNIEnv *env, jobject thiz, jstring url_) {

    const char* url = env->GetStringUTFChars(url_, 0);

    // TODO: implement n_playpcm()

    pcmFile = fopen(url, "r");
    if (pcmFile == NULL) {
        LOGE("open %s error! %d", url, errno);
        return ;
    }

    out_buffer = (uint8_t *)malloc((44100*2*2));

    // 1.创建接口对象
    slCreateEngine(&engineObject, 0, 0, 0, 0, 0);
    (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

    // 2.设置混音器
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

    (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB, &outputMixEnvReverb);
    (*outputMixEnvReverb)->SetEnvironmentalReverbProperties(outputMixEnvReverb, &reverbSettings);

    // 3.创建播放器
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,//播放pcm格式的数据
            2,//2个声道（立体声）
            SL_SAMPLINGRATE_44_1,//44100hz的频率
            SL_PCMSAMPLEFORMAT_FIXED_16,//位数 16位
            SL_PCMSAMPLEFORMAT_FIXED_16,//和位数一致就行
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN//结束标志
    };
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSource slDataSource = {&android_queue, &pcm};
    SLDataSink audioSink = {&outputMix, NULL};

    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};

    (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject,
            &slDataSource, &audioSink, 1, ids, req);
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);

    // 4.设置缓冲队列和回调函数
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallback, NULL);

    // 5.设置播放状态
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);

    // 6.启动回调函数
    pcmBufferCallback(pcmBufferQueue, NULL);

    env->ReleaseStringUTFChars(url_, url);
}