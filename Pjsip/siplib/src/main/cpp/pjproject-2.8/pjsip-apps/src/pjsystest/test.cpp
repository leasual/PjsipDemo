//
// Created by liuzl15 on 2019/10/25.
//
#include <string>
#include "main_console.h"


enum {
    ALL =0,
    PLAY,
    RECORD,
    DEVICE,
    CALC_LATENCY,
    AEC
};


static char* jstringTostring(JNIEnv* env, jstring jstr)
{
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = '\0';
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}


extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_native_1inits(
        JNIEnv *env,
        jobject /* this */) {
    ALOGE("-----inits");
    is_open_aec = PJ_FALSE;
    test_inits();
    return 0;
}

int aec_tailLength = 0;
int aec_options = 0;

extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_native_1module_1init(
        JNIEnv *env,
        jobject /*thiz*/,jint mode,jint arg) {
    ALOGE("-----mode:%d",mode);
    switch (mode){
        case ALL:
//            main();
//            gui_sleep(TEST_SLEEP);
//            menu_input("04");
            break;
        case PLAY:
            test_play_init(arg);
            break;
        case RECORD:
            test_record_init();
            break;
        case DEVICE:
            break;
        case CALC_LATENCY:
            break;
        case AEC:
            test_aec_init(aec_tailLength,aec_options,arg);
            break;
    }

}


extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_native_1module_1stop(
        JNIEnv *env,
        jobject /*thiz*/,jint mode) {

    switch (mode){
        case ALL:
            break;
        case PLAY:
            test_play_stop();
            break;
        case RECORD:
            test_record_stop();
            break;
        case DEVICE:
            break;
        case CALC_LATENCY:
            break;
        case AEC:
            test_aec_stop();
            break;
    }

}




extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_native_1module_1destroy(
        JNIEnv *env,
        jobject /*thiz*/,jint mode) {

    switch (mode){
        case ALL:
            break;
        case PLAY:
            test_play_destroy();
            break;
        case RECORD:
            test_record_destroy();
            break;
        case DEVICE:
            break;
        case CALC_LATENCY:
            break;
        case AEC:
            test_aec_destroy();
            break;
    }

}

extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_native_1destroy(
        JNIEnv *env,
        jobject /*thiz*/,jstring input) {
    ALOGE("-----destroy");
    _test_destroy();
}


extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_nativeMicLevel(
        JNIEnv *env,
        jobject /*thiz*/,jfloat micLevel) {
    ALOGE("native--micLevel :%f",micLevel);
        setMicLevel(micLevel);
}

extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_nativeSpeakerLever(
        JNIEnv *env,
        jobject /*thiz*/,jfloat speakerLevel) {
    ALOGE("native--speakerLevel :%d",speakerLevel);
    setSpeakerLevel(speakerLevel);
}

extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_nativeTailLength(
        JNIEnv *env,
        jobject /*thiz*/,jint tailLength) {
    ALOGE("native--tailLength :%d",tailLength);
    aec_tailLength = tailLength;

}
extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_nativeDelay(
        JNIEnv *env,
        jobject /*thiz*/,jint delay) {
    ALOGE("native--delay :%d",delay);
        set_delat_time(delay);
}
extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_nativeOptions(
        JNIEnv *env,
        jobject /*thiz*/,jint options) {
    ALOGE("native--options :%d",options);
        aec_options = options;
}


extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_nativeSetVad(
        JNIEnv *env,
        jobject /*thiz*/,jboolean noVad) {
    ALOGE("native--noVad :%d",noVad);
    setNoVad(noVad);
}

extern "C" JNIEXPORT void JNICALL
Java_com_csipsdk_sdk_native_1test_TestNative_nativeIsOpenAec(
        JNIEnv *env,
        jobject /*thiz*/,jboolean is_open) {
}