//
// Created by ky4910 on 2020/2/1.
//

#include "KyQueue.h"

KyQueue::KyQueue(KyPlayStatus *kyPlayStatus) {
    this->playStatus = kyPlayStatus;
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);
}

KyQueue::~KyQueue() {
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

int KyQueue::putAvPacket(AVPacket *packet) {

    pthread_mutex_lock(&mutexPacket);
    queuePacket.push(packet);
    if (LOG_DEBUG) {
        LOGD("放入一个AVPacket到队列中，个数为%d", queuePacket.size());
    }
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);

    return 0;
}

int KyQueue::getAvPacket(AVPacket *packet) {

    LOGE("get avPacket!");

    pthread_mutex_lock(&mutexPacket);

    while (playStatus!=NULL && !playStatus->exit) {
        if (queuePacket.size() > 0) {
            AVPacket *avPacket = queuePacket.front();
            // 引用拷贝
            if (av_packet_ref(packet, avPacket) == 0) {
                queuePacket.pop();
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            if (LOG_DEBUG) {
                LOGD("从队列里面取出一个AvPacket, 还剩下 %d 个", queuePacket.size());
            }
            break;
        } else {
            pthread_cond_wait(&condPacket, &mutexPacket);
        }
    }
    pthread_mutex_unlock(&mutexPacket);

    return 0;
}

int KyQueue::getQueueSize() {

    int size = 0;

    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);

    return size;
}
