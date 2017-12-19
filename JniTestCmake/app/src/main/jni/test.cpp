//
// Created by mark on 2017/12/18.
//
#include <jni.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mark_jnitestcmake_MainActivity_getString(JNIEnv *env){
    char *hello = "Hello from C++,test 1";
  return env->NewStringUTF(hello);
}
