//
// Created by ky4910 on 2020/2/1.
//

#ifndef KYNDKPLAYER_KYAUDIO_H
#define KYNDKPLAYER_KYAUDIO_H

#include "KyQueue.h"
#include "KyPlayStatus.h"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libswresample/swresample.h>
};

class KyAudio {

public:
    int streamIndex = -1;
    AVCodecParameters *codecPar = NULL;
    AVCodecContext *avCodecContext = NULL;
    KyQueue *kyQueue = NULL;
    KyPlayStatus *playStatus = NULL;

    pthread_t thread_play;
    AVPacket *avPacket = NULL;
    AVFrame *avFrame = NULL;
    int ret = -1;
    uint8_t *buffer = NULL;
    int data_size = 0;

public:
    KyAudio(KyPlayStatus *playStatus);
    ~KyAudio();

    void play();
    int resampleAudio();

};


#endif //KYNDKPLAYER_KYAUDIO_H
