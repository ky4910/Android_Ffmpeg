//
// Created by ky4910 on 2020/1/31.
//

#include "KyCallJava.h"

KyCallJava::KyCallJava(JavaVM *vm, JNIEnv *env, jobject *obj) {

    this->javaVm = vm;
    this->jniEnv = env;
    this->jobj = *obj;
    this->jobj = env->NewGlobalRef(jobj);

    jclass jlz = jniEnv->GetObjectClass(jobj);
    if (!jlz) {
        if (LOG_DEBUG) {
            LOGE("get jclass wrong!");
        }
        return ;
    }

    jmid_prepared = env->GetMethodID(jlz, "onCallPrepared", "()V");
}

KyCallJava::~KyCallJava() {}

void KyCallJava::onCallPrepared(int type) {

    if (type == MAIN_THREAD) {
        jniEnv->CallVoidMethod(jobj, jmid_prepared);
    } else if (type == CHILD_THREAD) {
        JNIEnv *jniEnv;
        if (javaVm->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            if (LOG_DEBUG) {
                LOGE("get child thread jnienv wrong!");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_prepared);
        javaVm->DetachCurrentThread();
    }

}

