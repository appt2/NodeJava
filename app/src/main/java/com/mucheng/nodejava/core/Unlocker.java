package com.mucheng.nodejava.core;

public class Unlocker {

    private final Isolate isolate;

    long unlockerPtr;

    public Unlocker(Isolate isolate) {
        this.isolate = isolate;
        nativeCreateUnlocker(isolate.isolatePtr);
    }

    public static Unlocker unlock(Isolate isolate) {
        return new Unlocker(isolate);
    }

    private native void nativeCreateUnlocker(long isolatePtr);

    public Isolate getIsolate() {
        return isolate;
    }

}
