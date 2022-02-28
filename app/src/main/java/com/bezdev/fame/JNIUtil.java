package com.bezdev.fame;

public class JNIUtil
{
    public native boolean IsAppReady();
    public native void SetAppScene(int sceneId);

    public static void Log(String message)
    {
        Logger.Log(message);
    }

    public static boolean IsReady() {
        return new JNIUtil().IsAppReady();
    }

    public static void SetScene(int sceneId) { new JNIUtil().SetAppScene(sceneId); }
}
