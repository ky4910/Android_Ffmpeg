//
// Created by ky4910 on 2020/1/31.
//

#include "JavaListener.h"


void JavaListener::onError(int type, int code, const char *msg) {

    // 子线程
    if(type == 0) {
        JNIEnv *env;
        jvm->AttachCurrentThread(&env, 0);
        jstring jmsg = env->NewStringUTF(msg);
        env->CallVoidMethod(jobj, jmid, code, jmsg);
        env->DeleteLocalRef(jmsg);

        jvm->DetachCurrentThread();
    }
    // 主线程
    else if(type == 1) {
        jstring jmsg = jenv->NewStringUTF(msg);
        jenv->CallVoidMethod(jobj, jmid, code, jmsg);
        jenv->DeleteLocalRef(jmsg);
    }
}

JavaListener::JavaListener(JavaVM *vm, _JNIEnv *env, jobject obj) {

    jvm = vm;
    jenv = env;
    jobj = obj;

    jclass clz = env->GetObjectClass(jobj);
    if(!clz) {
        return;
    }
    jmid = env->GetMethodID(clz, "onError", "(ILjava/lang/String;)V");
    if(!jmid)
        return;

}

JavaListener::~JavaListener() {

}

