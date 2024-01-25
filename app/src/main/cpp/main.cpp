#include <jni.h>
#include <uv.h>
#include <memory>
#include <string>
#include <node.h>
#include <pthread.h>
#include <unistd.h>
#include <android/log.h>
#include <nan.h>

#include "main.h"
#include "log.h"

int pipe_stdout[2];
int pipe_stderr[2];
pthread_t thread_stdout;
pthread_t thread_stderr;
const char *ADBTAG = "NodeJavaNative";

void *thread_stderr_func(void *) {
    ssize_t redirect_size;
    char buf[2048];
    while ((redirect_size = read(pipe_stderr[0], buf, sizeof buf - 1)) > 0) {
        //__android_log will add a new line anyway.
        if (buf[redirect_size - 1] == '\n')
            --redirect_size;
        buf[redirect_size] = 0;
        __android_log_write(ANDROID_LOG_ERROR, ADBTAG, buf);
    }
    return 0;
}

void *thread_stdout_func(void *) {
    ssize_t redirect_size;
    char buf[2048];
    while ((redirect_size = read(pipe_stdout[0], buf, sizeof buf - 1)) > 0) {
        //__android_log will add a new line anyway.
        if (buf[redirect_size - 1] == '\n')
            --redirect_size;
        buf[redirect_size] = 0;
        __android_log_write(ANDROID_LOG_VERBOSE, ADBTAG, buf);
    }
    return 0;
}

int start_redirecting_stdout_stderr() {
    //set stdout as unbuffered.
    setvbuf(stdout, 0, _IONBF, 0);
    pipe(pipe_stdout);
    dup2(pipe_stdout[1], STDOUT_FILENO);

    //set stderr as unbuffered.
    setvbuf(stderr, 0, _IONBF, 0);
    pipe(pipe_stderr);
    dup2(pipe_stderr[1], STDERR_FILENO);

    if (pthread_create(&thread_stdout, 0, thread_stdout_func, 0) == -1)
        return -1;
    pthread_detach(thread_stdout);

    if (pthread_create(&thread_stderr, 0, thread_stderr_func, 0) == -1)
        return -1;
    pthread_detach(thread_stderr);

    return 0;
}

JavaVM *Main::vm = nullptr;
node::InitializationResult *Main::initializationResult = nullptr;
node::MultiIsolatePlatform *Main::platform = nullptr;

JNIEnv *Main::env() {
    JNIEnv *env = nullptr;
    vm->AttachCurrentThread(&env, nullptr);
    return env;
}

jint JNI_OnLoad(JavaVM *vm, void *unused) {
    Main::vm = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    start_redirecting_stdout_stderr();

    std::vector<std::string> args = {"node"};

    Main::initializationResult =
            node::InitializeOncePerProcess(args, {
                    node::ProcessInitializationFlags::kNoInitializeV8,
                    node::ProcessInitializationFlags::kNoInitializeNodeV8Platform
            }).release();

    for (const std::string &error: Main::initializationResult->errors())
        LOGE("%s: %s\n", args[0].c_str(), error.c_str());
    if (Main::initializationResult->early_return() != 0) {
        return Main::initializationResult->exit_code();
    }


    Main::platform =
            node::MultiIsolatePlatform::Create(4).release();
    v8::V8::InitializePlatform(Main::platform);
    v8::V8::Initialize();

    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *vm, void *unused) {
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    node::TearDownOncePerProcess();

    Main::initializationResult = nullptr;
    Main::platform = nullptr;
    Main::vm = nullptr;
}