//
// Created by cain on 2018/12/9.
//

#include "JniMediaPlayerCallback.h"

JniMediaPlayerCallback::JniMediaPlayerCallback(_JavaVM *javaVM, JNIEnv *env, jobject *obj) {
    this->javaVM = javaVM;
    this->jniEnv = env;
    this->jobj = *obj;
    this->jobj = env->NewGlobalRef(jobj);

    jclass  clazz = jniEnv->GetObjectClass(jobj);
    if (clazz) {
        jmid_complete    = env->GetMethodID(clazz, "onCompletion", "()V");
        jmid_error       = env->GetMethodID(clazz, "onError", "(ILjava/lang/String;)V");
        jmid_prepared    = env->GetMethodID(clazz, "onPrepared", "()V");
        jmid_getPCM      = env->GetMethodID(clazz, "onGetPCM", "([BI)V");
    }
}

JniMediaPlayerCallback::~JniMediaPlayerCallback() {

}

void JniMediaPlayerCallback::onPrepared() {
    JNIEnv *jniEnv;
    if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        return;
    }

    jniEnv->CallVoidMethod(jobj, jmid_prepared);

    javaVM->DetachCurrentThread();
}

void JniMediaPlayerCallback::onError(int code, char *msg) {
    JNIEnv *jniEnv;
    if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        return;
    }

    jstring jmsg = jniEnv->NewStringUTF(msg);
    jniEnv->CallVoidMethod(jobj, jmid_error, code, jmsg);
    jniEnv->DeleteLocalRef(jmsg);

    javaVM->DetachCurrentThread();
}

void JniMediaPlayerCallback::onComplete() {
    JNIEnv *jniEnv;
    if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        return;
    }

    jniEnv->CallVoidMethod(jobj, jmid_complete);

    javaVM->DetachCurrentThread();
}

void JniMediaPlayerCallback::onGetPCM(uint8_t *pcmData, size_t size) {
    JNIEnv *jniEnv;
    if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        return;
    }

    jbyteArray data = jniEnv->NewByteArray(size);
    jniEnv->SetByteArrayRegion(data, 0, size, (jbyte*)pcmData);
    jniEnv->CallVoidMethod(jobj, jmid_getPCM, data, size);
    jniEnv->DeleteLocalRef(data);

    javaVM->DetachCurrentThread();
}
