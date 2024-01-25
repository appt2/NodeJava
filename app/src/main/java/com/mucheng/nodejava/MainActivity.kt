package com.mucheng.nodejava

import android.app.Activity
import android.os.Bundle
import com.mucheng.nodejava.core.Context
import com.mucheng.nodejava.core.Isolate

class MainActivity : Activity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val isolate = Isolate()
        val context = Context(isolate)
        context.injectJavaBridge()
        context.evaluateScript(readFromAssets("nodeJava.js"))
        context.evaluateScript(readFromAssets("test.js"))
    }

    private fun readFromAssets(path: String) = assets.open(path).bufferedReader().use { it.readText() }

}
