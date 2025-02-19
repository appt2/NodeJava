package com.mucheng.nodejava.core;

public class Context {

    private final Isolate isolate;

    long contextPtr;

    public Context(Isolate isolate) {
        this(isolate, null);
    }

    public Context(Isolate isolate, String source) {
        this.isolate = isolate;
        nativeCreateContext(isolate.isolatePtr);
        nativeLoadEnvironment(source);
    }

    public void evaluateScript(String script) {
        if (script == null) {
            throw new NullPointerException("script cannot be null");
        }
        nativeEvaluateScript(script);
    }

    public boolean spinEventLoop() {
        return nativeSpinEventLoop();
    }

    public void stop() {
        nativeStop();
    }

    public void emitProcessExit() {
        nativeEmitProcessExit();
    }

    public void injectJavaBridge() {
        nativeInjectJavaBridge();
    }

    private native void nativeCreateContext(long isolatePtr);

    private native void nativeLoadEnvironment(String source);

    private native void nativeEvaluateScript(String script);

    private native boolean nativeSpinEventLoop();

    private native void nativeStop();

    private native void nativeEmitProcessExit();

    private native void nativeInjectJavaBridge();

    public Isolate getIsolate() {
        return isolate;
    }

}
