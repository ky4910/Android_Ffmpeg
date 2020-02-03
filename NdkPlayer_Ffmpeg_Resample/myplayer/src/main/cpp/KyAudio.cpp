//
// Created by ky4910 on 2020/2/1.
//

#include "KyAudio.h"

KyAudio::KyAudio(KyPlayStatus *playStatus) {
    this->playStatus = playStatus;
    kyQueue = new KyQueue(playStatus);
    buffer = static_cast<uint8_t *>(av_malloc(44100 * 2 * 16));
}

KyAudio::~KyAudio() {

}

void *decodePlayCallback(void *data) {

    KyAudio *kyAudio = static_cast<KyAudio *>(data);
    kyAudio->resampleAudio();
    pthread_exit(&kyAudio->thread_play);
}

void KyAudio::play() {

    pthread_create(&thread_play, NULL, decodePlayCallback, this);

}

FILE *outFile = fopen("/mnt/sdcard/mymusic.pcm", "w");

int KyAudio::resampleAudio() {

    while (playStatus!=NULL && !playStatus->exit) {
        avPacket = av_packet_alloc();
        if (kyQueue->getAvPacket(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        ret = avcodec_send_packet(avCodecContext, avPacket);
        if (ret != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, avFrame);
        if (ret == 0) {
            if (avFrame->channels>0 && avFrame->channel_layout==0) {
                avFrame->channel_layout = av_get_default_channel_layout(avFrame->channels);
            } else if (avFrame->channels==0 && avFrame->channel_layout>0){
                avFrame->channels = av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }

            SwrContext *swr_ctx;
            swr_ctx = swr_alloc_set_opts(
                    NULL, AV_CH_LAYOUT_STEREO,
                    AV_SAMPLE_FMT_S16,
                    avFrame->sample_rate,
                    avFrame->channel_layout,
                    (AVSampleFormat)avFrame->format,
                    avFrame->sample_rate,
                    NULL, NULL);

            if (!swr_ctx || swr_init(swr_ctx)<0) {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = NULL;
                swr_free(&swr_ctx);
                continue;
            }

            int nb = swr_convert(swr_ctx, &buffer, avFrame->nb_samples,
                    (const uint8_t **)(avFrame->data), avFrame->nb_samples);
            int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
            data_size = nb * out_channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

            fwrite(buffer, 1, data_size, outFile);

            LOGE("data size is %d!", data_size);

            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            swr_free(&swr_ctx);
            swr_ctx = NULL;

        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            continue;
        }
    }

    fclose(outFile);

    return data_size;
}