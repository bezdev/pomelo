package com.bezdev.pomelo;

import android.util.Log;

import java.util.LinkedList;

public class Logger
{
    private static LinkedList<String> s_MemoryLog = new LinkedList<>();

    public static void Log(String message)
    {
        s_MemoryLog.add(message);
    }

    public static void ClearLog()
    {
        s_MemoryLog.clear();
    }

    public static LinkedList<String> GetLog()
    {
        return s_MemoryLog;
    }
}
