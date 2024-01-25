#ifndef NODEJAVA_MAIN_H
#define NODEJAVA_MAIN_H

#include <jni.h>
#include <node.h>

class Main {
public:
    static JavaVM *vm;
    static JNIEnv *env();
    static node::InitializationResult *initializationResult;
    static node::MultiIsolatePlatform *platform;
private:
    Main() = default;
};

#endif //NODEJAVA_MAIN_H
