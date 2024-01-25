package com.mucheng.nodejava.core;

public class Locker {

    private final Isolate isolate;

    long lockerPtr;

    public Locker(Isolate isolate) {
        this.isolate = isolate;
        nativeCreateLocker(isolate.isolatePtr);
    }

    public static Locker lock(Isolate isolate) {
        return new Locker(isolate);
    }

    public static boolean isLocked(Isolate isolate) {
        return nativeIsLocked(isolate.isolatePtr);
    }

    public void release() {
        nativeRelease();
    }

    public boolean isLocked() {
        return isLocked(isolate);
    }

    public boolean isActive() {
        return nativeIsActive();
    }

    public boolean wasEverUsed() {
        return nativeWasEverUsed();
    }

    private native void nativeCreateLocker(long isolatePtr);

    private static native boolean nativeIsLocked(long isolatePtr);

    private native void nativeRelease();

    private native boolean nativeIsActive();

    private native boolean nativeWasEverUsed();

    public Isolate getIsolate() {
        return isolate;
    }

}
