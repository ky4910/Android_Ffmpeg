#include <jni.h>
#include <string>

#include "pthread.h"
#include "AndroidLog.h"

pthread_t thread;

void *normalCallBack(void *data) {
    LOGD("create normal thread from C++!");
    pthread_exit(&thread);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_customjni_ThreadDemo_normalThread(JNIEnv *env, jobject thiz) {

    // TODO: implement normalThread()
    pthread_create(&thread, NULL, normalCallBack, NULL);

}

#include "queue"
#include "unistd.h"

pthread_t produc;
pthread_t custom;
pthread_mutex_t mutex;
pthread_cond_t cond;

std::queue<int> myQueue;

void *producCallback(void *data) {

    while(1) {
        pthread_mutex_lock(&mutex);
        myQueue.push(1);
        LOGD("生产者生产一个产品，通知消费者消费，产品数量为 %d", myQueue.size());
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(5);
    }
    pthread_exit(&produc);
}

void *customCallback(void *data) {

    while(1) {
        pthread_mutex_lock(&mutex);
        if (myQueue.size() > 0) {
            myQueue.pop();
            LOGD("消费者消费产品，产品数量还剩余 %d", myQueue.size());
        } else {
            LOGD("没有产品可以消费，等待中... ");
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        usleep(500 * 1000);
    }
    pthread_exit(&custom);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_customjni_ThreadDemo_mutexThread(JNIEnv *env, jobject thiz) {

    // TODO: implement mutexThread()
    for (int i = 0; i < 10; ++i) {
        myQueue.push(1);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&produc, NULL, producCallback, NULL);
    pthread_create(&custom, NULL, customCallback, NULL);
}

#include "JavaListener.h"
JavaVM *jvm;
JavaListener *javaListener;

pthread_t childThread;
void *childCallback(void *data) {
    JavaListener *javaListener = (JavaListener *)data;
    javaListener->onError(0, 101, "C++ call java method from child thread!");
    pthread_exit(&childThread);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_customjni_ThreadDemo_callbackFromC(JNIEnv *env, jobject thiz) {

    // TODO: implement callbackFromC()
    javaListener = new JavaListener(jvm, env, env->NewGlobalRef(thiz));
    // javaListener->onError(1, 100, "C++ call java method from main thread!");
    pthread_create(&childThread, NULL, childCallback, javaListener);
}

// 由于VM执行到System.loadLibrary()函数时，就会立即先呼叫JNI_OnLoad()，
// 所以C组件的开发者可以藉由JNI_OnLoad()来进行C组件内的初期值之设定(Initialization)
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env;
    jvm = vm;

    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    return JNI_VERSION_1_6;
}
