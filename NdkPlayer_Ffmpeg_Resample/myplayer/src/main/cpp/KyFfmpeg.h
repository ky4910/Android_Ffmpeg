//
// Created by ky4910 on 2020/2/1.
//

#ifndef KYNDKPLAYER_KYFFMPEG_H
#define KYNDKPLAYER_KYFFMPEG_H

#include "KyCallJava.h"
#include "pthread.h"
#include "KyAudio.h"

extern  "C" {
    #include <libavformat/avformat.h>
};

class KyFfmpeg {

public:
    KyCallJava *callJava = NULL;
    const char *url = NULL;
    pthread_t decodeThread;
    AVFormatContext *pFormatCtx = NULL;
    KyAudio *audio = NULL;
    KyPlayStatus *playStatus = NULL;

public:
    KyFfmpeg(KyPlayStatus *playStatus, KyCallJava *callJava, const char* url);
    ~KyFfmpeg();

    void prepared();
    void decodeFFmpegThread();
    void start();

};


#endif //KYNDKPLAYER_KYFFMPEG_H
