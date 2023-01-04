package com.bezdev.pomelo;

import android.content.Context;
import android.util.Log;

import androidx.test.ext.junit.rules.ActivityScenarioRule;
import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.LinkedList;

import static org.junit.Assert.*;

@RunWith(AndroidJUnit4.class)
public class PerformanceTest
{
    @Rule
    public ActivityScenarioRule<MainActivity> rule = new ActivityScenarioRule<>(MainActivity.class);

    @Before
    public void Setup()
    {
        Logger.ClearLog();

        JNIUtil.SetScene(2);

        while (!JNIUtil.IsReady()) { Sleep(500); };

        Log.d("test", "Setup done");

        JNIUtil.SetScene(4);
    }

    @After
    public void Teardown()
    {
        Log.d("test", "Teardown start");
        rule.getScenario().recreate();
        Log.d("test", "Teardown done");
    }

    @Test
    public void SingleCubeTest()
    {
        /*
        SceneManager.CreateScene(SceneManager.SceneType.SingleCube);
        Game.GetInstance().Resume();

        InputController.GetInstance().Drag(new Point(10, 900), new Point(1000, 900), 5000);
        Sleep(10000);
        float avgFPS = GetAverageFPS(Logger.GetLog());

        CheckFPS(avgFPS, 59.5f);
        */

        JNIUtil.SetScene(6);
        Log.d("test", "sleep start");
        Sleep(3000);
        Log.d("test", "sleep done");
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        LinkedList<String> log = Logger.GetLog();
        assertTrue(JNIUtil.IsReady());
        assertEquals("com.bezdev.pomelo", appContext.getPackageName());
    }


    @Test
    public void ManyCubeTest()
    {
        /*
        SceneManager.CreateScene(SceneManager.SceneType.SingleCube);
        Game.GetInstance().Resume();

        InputController.GetInstance().Drag(new Point(10, 900), new Point(1000, 900), 5000);
        Sleep(10000);
        float avgFPS = GetAverageFPS(Logger.GetLog());

        CheckFPS(avgFPS, 59.5f);
        */

        JNIUtil.SetScene(12);
        Log.d("test", "sleep start");
        Sleep(3000);
        Log.d("test", "sleep done");
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        LinkedList<String> log = Logger.GetLog();
        assertTrue(JNIUtil.IsReady());
        assertEquals("com.bezdev.pomelo", appContext.getPackageName());
    }

    public void Sleep(int ms)
    {
        try
        {
            Thread.sleep(ms);
        }
        catch (Exception e)
        {

        }
    }
}