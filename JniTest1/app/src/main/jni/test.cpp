//
// Created by mark on 2017/12/18.
//
#include <jni.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_jnitest1_mark_com_jnitest1_MainActivity_getString(JNIEnv *env){
    char *hello = "Hello from C++,test 1";
  return env->NewStringUTF(hello);
}
