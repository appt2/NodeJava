#ifndef NODEJAVA_ISOLATE_H
#define NODEJAVA_ISOLATE_H
#include <jni.h>
#include <v8.h>
#include <node.h>

#define SETUP_ISOLATE_CLASS() v8::Isolate::Scope isolateScope(isolate->self); \
v8::HandleScope handleScope(isolate->self);

class Isolate {
public:
    node::ArrayBufferAllocator *allocator;
    uv_loop_s *loop;
    v8::Isolate *self;
    node::IsolateData *isolateData;

    Isolate();

    static void To(jobject instance, Isolate *self);
    static Isolate* From(jobject instance);
};


#endif //NODEJAVA_ISOLATE_H
