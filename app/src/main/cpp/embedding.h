#ifndef NODEJAVA_EMBEDDING_H
#define NODEJAVA_EMBEDDING_H

#include <node.h>
#include <v8.h>

#define SETUP_CALLBACK_INFO() v8::Isolate *isolate = info.GetIsolate(); \
v8::Local<v8::Context> context = isolate->GetCurrentContext();

void JAVA_ACCESSOR_BINDING(
        v8::Local<v8::Object> exports,
        v8::Local<v8::Value> module,
        v8::Local<v8::Context> context,
        void *priv
);

#endif //NODEJAVA_EMBEDDING_H
