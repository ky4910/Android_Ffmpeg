//
// Created by ky4910 on 2020/2/1.
//

#ifndef KYNDKPLAYER_KYQUEUE_H
#define KYNDKPLAYER_KYQUEUE_H

#include "queue"
#include "pthread.h"
#include "KyPlayStatus.h"
#include "AndroidLog.h"


extern "C" {
    #include "libavcodec/avcodec.h"
};

using namespace std;

class KyQueue {

public:
    queue<AVPacket *> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    KyPlayStatus *playStatus = NULL;

public:
    KyQueue(KyPlayStatus *playStatus);
    ~KyQueue();

    int putAvPacket(AVPacket *packet);
    int getAvPacket(AVPacket *packet);
    int getQueueSize();

};


#endif //KYNDKPLAYER_KYQUEUE_H
