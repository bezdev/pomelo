#include "jniutil.h"

#ifdef BUILD_ANDROID
JNIUtil::JNIUtil()
{
}

JNIUtil::~JNIUtil() {
    //DetachCurrentThread();
    LOGD("JNIUtil::~JNIUtil");
}

void JNIUtil::Initialize(App* app)
{
    //ANativeActivity* activity,
    //m_Activity = activity;
    m_App = app;
}

//void JNIUtil::Initialize(ANativeActivity* activity, App* app)
//{
//    //ANativeActivity* activity,
//    m_Activity = activity;
//    m_App = app;
//}

JNIEnv* JNIUtil::AttachCurrentThread()
{
    JNIEnv* env;
    m_Activity->vm->AttachCurrentThread(&env, NULL);
    return env;
}

void JNIUtil::DetachCurrentThread()
{
    m_Activity->vm->DetachCurrentThread();
}

void JNIUtil::LogTrigger(const char* message)
{
#ifdef TEST
    if (m_JNILogThread.joinable())
    {
    //    m_JNILogThread.join();
    }
    //m_JNILogThread = std::thread([this, message]()
    //{
        JNIUtil::GetInstance()->Log(message);
    //});
#endif
}

void JNIUtil::Log(const char* message)
{
//    auto env = AttachCurrentThread();
//    jobject nativeActivity = m_Activity->clazz;
//    jclass activity = env->GetObjectClass(nativeActivity);
//    jmethodID getClassLoader = env->GetMethodID(activity, "getClassLoader", "()Ljava/lang/ClassLoader;");
//    jobject cls = env->CallObjectMethod(nativeActivity, getClassLoader);
//    jclass classLoader = env->FindClass("java/lang/ClassLoader");
//    jmethodID findClass = env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
//    jstring className = env->NewStringUTF("com.bezdev.pomelo.JNIUtil");
//    jclass jniUtilClass = (jclass)(env->CallObjectMethod(cls, findClass, className));
//    jmethodID method = env->GetStaticMethodID(jniUtilClass, "Log", "(Ljava/lang/String;)V");
//    jstring jMessage = env->NewStringUTF(message);
//    env->CallStaticVoidMethod(jniUtilClass, method, jMessage);
//    env->DeleteLocalRef(jMessage);
//    env->DeleteLocalRef(jniUtilClass);
//    env->DeleteLocalRef(className);
//    env->DeleteLocalRef(classLoader);
//    env->DeleteLocalRef(cls);
//    env->DeleteLocalRef(activity);
//    DetachCurrentThread();
}

extern "C" JNIEXPORT bool JNICALL Java_com_bezdev_pomelo_JNIUtil_IsAppReady(JNIEnv* env, jobject)
{
    return false; // App::GetInstance()->IsReady();
}

extern "C" JNIEXPORT void JNICALL Java_com_bezdev_pomelo_JNIUtil_SetAppScene(JNIEnv* env, jobject, jint sceneId)
{
    LOGI("==================Java_com_bezdev_pomelo_JNIUtil_SetScene: %d", sceneId);
}
#endif
