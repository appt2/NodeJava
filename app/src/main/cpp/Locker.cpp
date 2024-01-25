#include "Locker.h"
#include "Util.h"
#include "Isolate.h"
#include <jni.h>

Locker::Locker(Isolate *isolate) {
    this->isolate = isolate;

    self = new v8::Locker(isolate->self);
}

void Locker::To(jobject instance, Locker *self) {
    Util::SetPtr(instance, "lockerPtr", self);
}

Locker *Locker::From(jobject instance) {
    return Util::GetPtrAs<Locker *>(instance, "lockerPtr");
}

bool Locker::isLocked(Isolate *isolate) {
    return v8::Locker::IsLocked(isolate->self);
}

void Locker::release() {
    delete this->self;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Locker_nativeCreateLocker(JNIEnv *env, jobject thiz,
                                                         jlong isolatePtr) {
    Isolate *isolate = Util::As<Isolate *>(isolatePtr);
    Locker *locker = new Locker(isolate);
    Locker::To(thiz, locker);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_mucheng_nodejava_core_Locker_nativeIsLocked(JNIEnv *env, jclass clazz, jlong isolatePtr) {
    Isolate *isolate = Util::As<Isolate *>(isolatePtr);
    return Locker::isLocked(isolate);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_mucheng_nodejava_core_Locker_nativeIsActive(JNIEnv *env, jobject thiz) {
    return Locker::From(thiz)->self->IsActive();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_mucheng_nodejava_core_Locker_nativeWasEverUsed(JNIEnv *env, jobject thiz) {
    return Locker::From(thiz)->self->WasEverUsed();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Locker_nativeRelease(JNIEnv *env, jobject thiz) {
    Locker::From(thiz)->release();
}