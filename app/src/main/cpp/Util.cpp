#include <jni.h>
#include "Util.h"
#include "main.h"

void Util::SetPtr(jclass javaClass, const char *fieldName, void *ptr) {
    jfieldID field = Main::env()->GetStaticFieldID(javaClass, fieldName, "J");
    Main::env()->SetStaticLongField(javaClass, field, reinterpret_cast<jlong>(ptr));
}

void Util::SetPtr(jobject instance, const char *fieldName, void *ptr) {
    jclass javaClass = Main::env()->GetObjectClass(instance);
    jfieldID field = Main::env()->GetFieldID(javaClass, fieldName, "J");
    Main::env()->SetLongField(instance, field, reinterpret_cast<jlong>(ptr));
}

void *Util::GetPtr(jclass javaClass, const char *fieldName) {
    jfieldID field = Main::env()->GetStaticFieldID(javaClass, fieldName, "J");
    return reinterpret_cast<void *>(Main::env()->GetStaticLongField(javaClass, field));
}

void *Util::GetPtr(jobject instance, const char *fieldName) {
    jclass javaClass = Main::env()->GetObjectClass(instance);
    jfieldID field = Main::env()->GetFieldID(javaClass, fieldName, "J");
    return reinterpret_cast<void *>(Main::env()->GetLongField(instance, field));
}

const char *Util::JavaStr2CStr(jstring javaStr) {
    return Main::env()->GetStringUTFChars(javaStr, nullptr);
}

jstring Util::CStr2JavaStr(const char *cStr) {
    return Main::env()->NewStringUTF(cStr);
}

void Util::ThrowScriptCompilingException(const char *message) {
    Main::env()->ThrowNew(
            Main::env()->FindClass("com/mucheng/nodejava/core/exception/ScriptCompilingException"),
            message
    );
}

void Util::ThrowScriptRuntimeException(const char *message) {
    Main::env()->ThrowNew(
            Main::env()->FindClass("com/mucheng/nodejava/core/exception/ScriptRuntimeException"),
            message
    );
}

void Util::ThrowV8Exception(const char *message) {
    Main::env()->ThrowNew(
            Main::env()->FindClass("com/mucheng/nodejava/core/exception/V8Exception"),
            message
    );
}

void Util::ThrowNodeException(const char *message) {
    Main::env()->ThrowNew(
            Main::env()->FindClass("com/mucheng/nodejava/core/exception/NodeException"),
            message
    );
}
