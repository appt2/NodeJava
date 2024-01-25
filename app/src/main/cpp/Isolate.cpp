#include "Isolate.h"
#include "Util.h"
#include "main.h"
#include <jni.h>
#include <uv.h>

Isolate::Isolate() {
    allocator = node::CreateArrayBufferAllocator();
    loop = uv_loop_new();
    self = node::NewIsolate(
            allocator,
            loop,
            Main::platform
    );
    isolateData = node::CreateIsolateData(
            self,
            loop,
            Main::platform,
            allocator
    );
}

void Isolate::To(jobject instance, Isolate *self) {
    Util::SetPtr(instance, "isolatePtr", self);
}

Isolate *Isolate::From(jobject instance) {
    return Util::GetPtrAs<Isolate *>(instance, "isolatePtr");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Isolate_nativeCreateIsolate(JNIEnv *env, jobject thiz) {
    Isolate *isolate = new Isolate();
    Isolate::To(thiz, isolate);
}