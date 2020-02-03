//
// Created by ky4910 on 2020/2/1.
//

#include "KyFfmpeg.h"

KyFfmpeg::KyFfmpeg(KyPlayStatus *kyPlayStatus, KyCallJava *callJava, const char *url) {

    this->callJava = callJava;
    this->url = url;
    this->playStatus = kyPlayStatus;

}

void *decodeFFmpeg(void *data) {
    KyFfmpeg *kyFfmpeg = (KyFfmpeg *)data;
    kyFfmpeg->decodeFFmpegThread();
    pthread_exit(&kyFfmpeg->decodeThread);
}

void KyFfmpeg::prepared() {

    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);

}

void KyFfmpeg::decodeFFmpegThread() {

    int err_code = 0;
    char buf[1024] = {'\0'};

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();
    err_code = avformat_open_input(&pFormatCtx, url, NULL, NULL);
    if (err_code != 0) {
        if (LOG_DEBUG) {
            av_strerror(err_code, buf, 1024);
            LOGE("cannot open url : %s", url);
            LOGE("error code: %d, error msg: %s", err_code, buf);
        }
        return ;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        if (LOG_DEBUG) {
            LOGE("cannot find stream from url :%s", url);
            return ;
        }
    }

    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (audio == NULL) {
                audio = new KyAudio(playStatus);
                audio->streamIndex = i;
                audio->codecPar = pFormatCtx->streams[i]->codecpar;
            }
        }
    }

    AVCodec *avCodec = avcodec_find_decoder(audio->codecPar->codec_id);
    if (!avCodec) {
        if (LOG_DEBUG) {
            LOGE("cannot find decoder!");
        }
        return ;
    }

    audio->avCodecContext = avcodec_alloc_context3(avCodec);
    if (!audio->avCodecContext) {
        if (LOG_DEBUG) {
            LOGE("cannot alloc new decodeContext!");
        }
        return ;
    }

    if (avcodec_parameters_to_context(audio->avCodecContext, audio->codecPar) < 0) {
        if (LOG_DEBUG) {
            LOGE("cannot fill decoderCtx!");
        }
        return ;
    }

    if (avcodec_open2(audio->avCodecContext, avCodec, 0) != 0) {
        if (LOG_DEBUG) {
            LOGE("cannot open audio streams!");
        }
        return ;
    }

    callJava->onCallPrepared(CHILD_THREAD);

}

KyFfmpeg::~KyFfmpeg() {

}

void KyFfmpeg::start() {

    if (audio == NULL) {
        if (LOG_DEBUG) {
            LOGE("audio is null!");
        }
        return ;
    }

    audio->play();

    int count = 0;

    while (playStatus!=NULL && !playStatus->exit) {
        AVPacket *avPacket = av_packet_alloc();
        if (av_read_frame(pFormatCtx, avPacket) == 0) {
            if (avPacket->stream_index == audio->streamIndex) {
                count ++;
                if (LOG_DEBUG) {
                    LOGE("解码第 %d 帧", count);
                }
                audio->kyQueue->putAvPacket(avPacket);
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            LOGE("decode finish!");
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            while (playStatus != NULL && !playStatus->exit) {
                if (audio->kyQueue->getQueueSize() > 0) {
                    continue;
                } else {
                    LOGE("set play status as true!");
                    playStatus->exit = true;
                    break;
                }
            }
        }
    }

    /*
    LOGE("before getAvPacket operation!");
    while (audio->kyQueue->getQueueSize() > 0) {
        AVPacket *avPacket = av_packet_alloc();
        audio->kyQueue->getAvPacket(avPacket);
        av_packet_free(&avPacket);
        av_free(avPacket);
        avPacket = NULL;
    }
     */

    if (LOG_DEBUG) {
        LOGD("解码完成！");
    }
}
