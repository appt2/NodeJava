#ifndef NODEJAVA_CONTEXT_H
#define NODEJAVA_CONTEXT_H

#include "Isolate.h"
#include <jni.h>

#define SETUP_CONTEXT_CLASS() v8::Context::Scope contextScope(context->self.Get(isolate->self));

class Context {
public:
    Isolate *isolate;
    node::Environment *environment;
    v8::Persistent<v8::Context> self;

    Context(Isolate *isolate);

    static void To(jobject instance, Context *self);

    static Context *From(jobject instance);
};


#endif //NODEJAVA_CONTEXT_H
