package com.mucheng.nodejava

import android.app.Activity
import android.os.Bundle
import com.mucheng.nodejava.core.Context
import com.mucheng.nodejava.core.Isolate
import kotlin.concurrent.thread

class MainActivity : Activity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val isolate = Isolate()
        val context = Context(isolate)

        thread {
            Thread.sleep(1000L)
            context.emitProcessExit()
        }

        context.evaluateScript("setInterval(() => console.log('ticked'), 200);")
        context.spinEventLoop()

        context.evaluateScript("console.log('exec done')")
    }

}
