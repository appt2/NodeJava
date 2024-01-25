#include "Context.h"
#include "Isolate.h"
#include "Util.h"
#include "main.h"
#include "log.h"
#include "embedding.h"
#include <jni.h>

Context::Context(Isolate *isolate) {
    this->isolate = isolate;

    SETUP_ISOLATE_CLASS();
    self.Reset(isolate->self, node::NewContext(isolate->self));

    environment = node::CreateEnvironment(
            isolate->isolateData,
            self.Get(isolate->self),
            Main::initializationResult->args(),
            Main::initializationResult->exec_args(),
            node::EnvironmentFlags::kOwnsProcessState
    );
}

void Context::To(jobject instance, Context *self) {
    Util::SetPtr(instance, "contextPtr", self);
}

Context *Context::From(jobject instance) {
    return Util::GetPtrAs<Context *>(instance, "contextPtr");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Context_nativeCreateContext(JNIEnv *env, jobject thiz,
                                                           jlong isolatePtr) {
    Isolate *isolate = Util::As<Isolate *>(isolatePtr);
    Context *context = new Context(isolate);
    Context::To(thiz, context);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Context_nativeLoadEnvironment(JNIEnv *env, jobject thiz,
                                                             jstring source) {
    Context *context = Context::From(thiz);
    Isolate *isolate = context->isolate;

    SETUP_ISOLATE_CLASS();
    SETUP_CONTEXT_CLASS();

    v8::TryCatch tryCatch(isolate->self);
    SetProcessExitHandler(context->environment, [](node::Environment *environment, int exitCode) {
        Stop(environment, node::StopFlags::kDoNotTerminateIsolate);
        LOGE("Process ExitCode: %d", exitCode);
    });

    v8::MaybeLocal<v8::Value> result;
    if (source == nullptr) {
        result = LoadEnvironment(context->environment,
                                 [&](const node::StartExecutionCallbackInfo &info) -> v8::MaybeLocal<v8::Value> {
                                     v8::EscapableHandleScope escapableHandleScope(isolate->self);
                                     return escapableHandleScope.Escape(
                                             v8::Undefined(isolate->self));
                                 });
    } else {
        result = LoadEnvironment(context->environment, Util::JavaStr2CStr(source));
    }

    if (result.IsEmpty()) {
        if (tryCatch.HasCaught()) {
            v8::MaybeLocal<v8::Value> stackTrace = v8::TryCatch::StackTrace(
                    context->self.Get(isolate->self), tryCatch.Exception());
            if (stackTrace.IsEmpty()) {
                LOGE("Uncaught Environment Error: %s",
                     *v8::String::Utf8Value(isolate->self, tryCatch.Exception()));
            } else {
                Util::ThrowNodeException(
                        *v8::String::Utf8Value(isolate->self, stackTrace.ToLocalChecked()));
            }
        }
        return;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_mucheng_nodejava_core_Context_nativeSpinEventLoop(JNIEnv *env, jobject thiz) {
    Context *context = Context::From(thiz);
    return SpinEventLoop(context->environment).FromMaybe(1) == 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Context_nativeStop(JNIEnv *env, jobject thiz) {
    Context *context = Context::From(thiz);
    Stop(context->environment, node::StopFlags::kDoNotTerminateIsolate);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Context_nativeEmitProcessExit(JNIEnv *env, jobject thiz) {
    Context *context = Context::From(thiz);
    EmitProcessExit(context->environment);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Context_nativeEvaluateScript(JNIEnv *env, jobject thiz,
                                                            jstring script) {
    Context *context = Context::From(thiz);
    Isolate *isolate = context->isolate;

    SETUP_ISOLATE_CLASS();
    SETUP_CONTEXT_CLASS();

    v8::TryCatch tryCatch(isolate->self);
    SetProcessExitHandler(context->environment, [](node::Environment *environment, int exitCode) {
        Stop(environment, node::StopFlags::kDoNotTerminateIsolate);
        LOGE("Process ExitCode: %d", exitCode);
    });

    v8::MaybeLocal<v8::Script> compiling = v8::Script::Compile(context->self.Get(isolate->self),
                                                               v8::String::NewFromUtf8(
                                                                       isolate->self,
                                                                       Util::JavaStr2CStr(
                                                                               script)).ToLocalChecked());
    if (compiling.IsEmpty()) {
        if (tryCatch.HasCaught()) {
            v8::MaybeLocal<v8::Value> stackTrace = v8::TryCatch::StackTrace(
                    context->self.Get(isolate->self), tryCatch.Exception());
            if (stackTrace.IsEmpty()) {
                LOGE("Uncaught Compiling Error: %s",
                     *v8::String::Utf8Value(isolate->self, tryCatch.Exception()));
            } else {
                Util::ThrowScriptCompilingException(
                        *v8::String::Utf8Value(isolate->self, stackTrace.ToLocalChecked()));
            }
        }
        return;
    }

    v8::MaybeLocal<v8::Value> running = compiling.ToLocalChecked()->Run(
            context->self.Get(isolate->self));

    if (running.IsEmpty()) {
        if (tryCatch.HasCaught()) {
            v8::MaybeLocal<v8::Value> stackTrace = v8::TryCatch::StackTrace(
                    context->self.Get(isolate->self), tryCatch.Exception());
            if (stackTrace.IsEmpty()) {
                LOGE("Uncaught Runtime Error: %s",
                     *v8::String::Utf8Value(isolate->self, tryCatch.Exception()));
            } else {
                Util::ThrowScriptRuntimeException(
                        *v8::String::Utf8Value(isolate->self, stackTrace.ToLocalChecked()));
            }
        }
        return;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mucheng_nodejava_core_Context_nativeInjectJavaBridge(JNIEnv *env, jobject thiz) {
    Context *context = Context::From(thiz);
    AddLinkedBinding(
            context->environment,
            "java",
            JAVA_ACCESSOR_BINDING,
            nullptr
    );
}