//
// Created by 35785 on 2024/1/24.
//

#ifndef NODEJAVA_UTIL_H
#define NODEJAVA_UTIL_H

namespace Util {
    void SetPtr(jclass javaClass, const char *fieldName, void *ptr);

    void SetPtr(jobject instance, const char *fieldName, void *ptr);

    void *GetPtr(jclass javaClass, const char *fieldName);

    void *GetPtr(jobject instance, const char *fieldName);

    void ThrowScriptCompilingException(const char *message);

    void ThrowScriptRuntimeException(const char *message);

    void ThrowV8Exception(const char *message);

    void ThrowNodeException(const char *message);

    template<typename Class>
    inline Class Cast(long ptr) {
        return reinterpret_cast<Class>(ptr);
    }

    template<typename Class>
    inline Class GetPtrCast(jclass javaClass, const char *fieldName) {
        return static_cast<Class>(GetPtr(javaClass, fieldName));
    }

    template<typename Class>
    inline Class GetPtrCast(jobject instance, const char *fieldName) {
        return static_cast<Class>(GetPtr(instance, fieldName));
    }

    const char *JavaStr2CStr(jstring javaStr);

    jstring CStr2JavaStr(const char *cStr);
}

#endif //NODEJAVA_UTIL_H
