//
// Created by ky4910 on 2020/1/31.
//

#include "jni.h"
#ifndef CUSTOMJNI_JAVALISTENER_H
#define CUSTOMJNI_JAVALISTENER_H

class JavaListener {

public:
    JavaVM *jvm;
    _JNIEnv *jenv;
    jobject jobj;
    jmethodID jmid;

public:
    JavaListener(JavaVM *vm, _JNIEnv *env, jobject obj);
    ~JavaListener();

    /**
     * 0: 子线程
     * 1: 主线程
     * @param type
     * @param code
     * @param msg
     */
    void onError(int type, int code, const char *msg);

};

#endif //CUSTOMJNI_JAVALISTENER_H
