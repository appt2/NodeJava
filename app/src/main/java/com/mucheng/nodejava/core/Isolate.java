package com.mucheng.nodejava.core;

public class Isolate {

    long isolatePtr;

    static {
        System.loadLibrary("nodejava");
    }

    public Isolate() {
        nativeCreateIsolate();
    }

    private native void nativeCreateIsolate();

}
