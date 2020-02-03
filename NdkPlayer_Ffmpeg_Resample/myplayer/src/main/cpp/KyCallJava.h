//
// Created by ky4910 on 2020/1/31.
//

#ifndef KYNDKPLAYER_KYCALLJAVA_H
#define KYNDKPLAYER_KYCALLJAVA_H

#include "jni.h"
#include <linux/stddef.h>
#include "AndroidLog.h"

#define MAIN_THREAD 0
#define CHILD_THREAD 1

class KyCallJava {

public:
    JavaVM *javaVm;
    JNIEnv *jniEnv;
    jobject jobj;

    jmethodID jmid_prepared;

public:
    KyCallJava(JavaVM *vm, JNIEnv *env, jobject *obj);
    ~KyCallJava();

    void onCallPrepared(int type);
};


#endif //KYNDKPLAYER_KYCALLJAVA_H
