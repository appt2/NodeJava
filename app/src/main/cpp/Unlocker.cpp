#include "Unlocker.h"
#include "Util.h"
#include <v8.h>

Unlocker::Unlocker(Isolate *isolate) {
    this->isolate = isolate;

    self = new v8::Unlocker(isolate->self);
}

void Unlocker::To(jobject instance, Unlocker *unlocker) {
    Util::SetPtr(instance, "unlockerPtr", unlocker);
}

Unlocker *Unlocker::From(jobject instance) {
    return Util::GetPtrAs<Unlocker *>(instance, "unlockerPtr");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Unlocker_nativeCreateUnlocker(JNIEnv *env, jobject thiz,
                                                             jlong isolatePtr) {
    Isolate *isolate = Util::As<Isolate *>(isolatePtr);
    Unlocker *unlocker = new Unlocker(isolate);
    Unlocker::To(thiz, unlocker);
}