//
// Created by ky4910 on 2020/1/31.
//

#ifndef CUSTOMJNI_ANDROIDLOG_H
#define CUSTOMJNI_ANDROIDLOG_H

#include <android/log.h>

#define LOGD(FORMAT,...) __android_log_print(ANDROID_LOG_DEBUG, "ywl5320", FORMAT, ##__VA_ARGS__);

#endif //CUSTOMJNI_ANDROIDLOG_H
