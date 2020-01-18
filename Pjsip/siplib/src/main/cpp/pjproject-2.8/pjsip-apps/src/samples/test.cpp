//
// Created by liuzl15 on 2019/11/7.
//
#include <jni.h>
#include <string.h>
#include <malloc.h>
#include "aectest.h"
#include "simple_pjsua.h"


static char* jstringToChar(JNIEnv* env,jstring jstr){
    const char *c_str = NULL;
    char buff[128] = {0};
    c_str = env->GetStringUTFChars(jstr,NULL);
    if(c_str == NULL)
    {
        return NULL;
    }
    sprintf(buff, "%s", c_str);
    env->ReleaseStringUTFChars(jstr, c_str);
    return buff;
}

static char* jstringTostring(JNIEnv* env, jstring jstr)
{
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    if(mid ==NULL){
        return NULL;
    }
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
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeAecTest(
        JNIEnv *env,
        jobject /* this */,jint tail_length,jint delay,jint options) {
    ALOGE("-------------- native Aec Test --------");
    char *play_path = "/sdcard/play.wav";
    char *rec_path = "/sdcard/play.WAV";
    char *out_path = "/sdcard/out.WAV";
    return test_main(play_path,rec_path,out_path,tail_length,delay,options);
}

JavaVM *g_VM;
jobject g_obj;

void call_cb(pj_bool_t success,int type,char *message){
    JNIEnv *env;
    int mNeedDetach =0;
    //获取当前native线程是否有没有被附加到jvm环境中
    int getEnvStat = g_VM->GetEnv((void **) &env,JNI_VERSION_1_4);
    if (getEnvStat == JNI_EDETACHED) {
        //如果没有， 主动附加到jvm环境中，获取到env
        if (g_VM->AttachCurrentThread(&env, NULL) != 0) {
            return;
        }
        mNeedDetach = JNI_TRUE;
    }

    //通过全局变量g_obj 获取到要回调的类
    jclass javaClass = env->GetObjectClass(g_obj);

    if (javaClass == 0) {
        ALOGE("Unable to find class");
        g_VM->DetachCurrentThread();
        return;
    }
    //获取要回调的方法ID
    jmethodID javaCallbackId = env->GetMethodID(javaClass,
                                                "onOperate", "(ZILjava/lang/String;)V");
    if (javaCallbackId == NULL) {
        ALOGE("Unable to find method:onProgressCallBack");
        return;
    }
    //执行回调
    env->CallVoidMethod(g_obj, javaCallbackId,success,type,env->NewStringUTF(message));

    //释放当前线程
    if(mNeedDetach) {
        g_VM->DetachCurrentThread();
    }
    env = NULL;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeRegister(
        JNIEnv *env,jobject /* this */,jstring user_name,jstring sip_no,jstring psw,jstring domain) {
    _test_reg_init();
    pjsua_var.test_call_cb = &call_cb;
    return _register(jstringTostring(env,user_name),jstringTostring(env,sip_no),jstringTostring(env,psw),jstringTostring(env,domain));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeMakecall(
        JNIEnv *env,jobject /* this */,jstring callNo) {
    return make_call(jstringTostring(env,callNo));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeAnswer(
        JNIEnv *env,jobject /* this */) {
    return test_answer(200);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeCancel(
        JNIEnv *env,jobject /* this */) {
    return o_hangup_call();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeReject(
        JNIEnv *env,jobject /* this */) {
    return i_hangup_call();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeHangup(
        JNIEnv *env,jobject /* this */) {
    return o_hangup_call();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeSendDTMF(
        JNIEnv *env,jobject /* this */,jstring dtmf_message) {
    return test_i_send_dtmf(jstringTostring(env,dtmf_message));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeRecDTMF(
        JNIEnv *env,jobject /* this */) {
    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeCallTestDestroy(
        JNIEnv *env,jobject /* this */) {
    return test_call_destroy();
}



extern "C" JNIEXPORT jint JNICALL
Java_com_csipsdk_sdk_native_1test_ModuleTestNative_nativeSetCallback(
        JNIEnv *env,jobject /* this */,jobject callback) {
    //JavaVM是虚拟机在JNI中的表示，等下再其他线程回调java层需要用到
    env->GetJavaVM(&g_VM);
    // 生成一个全局引用保留下来，以便回调
    g_obj = env->NewGlobalRef(callback);
    return 0;
}




