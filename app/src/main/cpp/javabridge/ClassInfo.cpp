#include "ClassInfo.h"
#include "main.h"
#include "Util.h"

v8::Local<v8::Object>
ClassInfo::BuildObject(v8::Isolate *isolate, v8::Local<v8::Context> context, jobject instance) {
    JNIEnv *env = Main::env();

    v8::Local<v8::Object> classInfo = v8::Object::New(isolate);

    jclass classInfoClass = env->FindClass("com/mucheng/nodejava/javabridge/ClassInfo");
    jclass methodClass = env->FindClass("java/lang/reflect/Method");
    jclass fieldClass = env->FindClass("java/lang/reflect/Field");
    jclass modifierClass = env->FindClass("java/lang/reflect/Modifier");
    jmethodID getMethodName = env->GetMethodID(methodClass, "getName", "()Ljava/lang/String;");
    jmethodID getFieldName = env->GetMethodID(fieldClass, "getName", "()Ljava/lang/String;");
    jmethodID getModifiers = env->GetMethodID(fieldClass, "getModifiers", "()I");
    jmethodID isFinal = env->GetStaticMethodID(modifierClass, "isFinal", "(I)Z");
    jfieldID classNameField = env->GetFieldID(classInfoClass, "className", "Ljava/lang/String;");
    jfieldID superclassField = env->GetFieldID(classInfoClass, "superclass", "Ljava/lang/String;");
    jfieldID declaredClassesField = env->GetFieldID(classInfoClass, "declaredClasses",
                                                    "[Ljava/lang/String;");
    jfieldID methodsField = env->GetFieldID(classInfoClass, "methods",
                                            "[Ljava/lang/reflect/Method;");
    jfieldID fieldsField = env->GetFieldID(classInfoClass, "fields", "[Ljava/lang/reflect/Field;");
    jfieldID staticMethodsField = env->GetFieldID(classInfoClass, "staticMethods",
                                                  "[Ljava/lang/reflect/Method;");
    jfieldID staticFieldsField = env->GetFieldID(classInfoClass, "staticFields",
                                                 "[Ljava/lang/reflect/Field;");
    jfieldID isArrayField = env->GetFieldID(classInfoClass, "isArray", "Z");

    v8::Local<v8::String> className = v8::String::NewFromUtf8(isolate, Util::JavaStr2CStr(
            static_cast<jstring>(env->GetObjectField(instance, classNameField))
    )).ToLocalChecked();

    jobject superclassValue = env->GetObjectField(instance, superclassField);
    v8::Local<v8::Value> superclass;
    if (superclassField == nullptr) {
        superclass = v8::Null(isolate);
    } else {
        superclass = v8::String::NewFromUtf8(isolate, Util::JavaStr2CStr(
                static_cast<jstring>(superclassValue))).ToLocalChecked();
    }

    jobjectArray declaredClassesValue = static_cast<jobjectArray>(env->GetObjectField(instance,
                                                                                      declaredClassesField));
    int declaredClassesLength = env->GetArrayLength(declaredClassesValue);
    v8::Local<v8::Array> declaredClasses = v8::Array::New(isolate, declaredClassesLength);
    for (int index = 0; index < declaredClassesLength; index++) {
        declaredClasses->Set(context, index,
                             v8::String::NewFromUtf8(isolate, Util::JavaStr2CStr(
                                     static_cast<jstring>(env->GetObjectArrayElement(
                                             declaredClassesValue,
                                             index)))).ToLocalChecked()).Check();
    }

    jobjectArray methodsValue = static_cast<jobjectArray>(env->GetObjectField(instance,
                                                                              methodsField));
    int methodLength = env->GetArrayLength(methodsValue);
    v8::Local<v8::Array> methods = v8::Array::New(isolate, methodLength);
    for (int index = 0; index < methodLength; index++) {
        v8::Local<v8::Object> methodObject = v8::Object::New(isolate);
        jobject methodInstance = env->GetObjectArrayElement(
                methodsValue,
                index);

        methodObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "name"),
                          v8::String::NewFromUtf8(isolate, Util::JavaStr2CStr(
                                  static_cast<jstring>(env->CallObjectMethod(methodInstance,
                                                                             getMethodName)))).ToLocalChecked()).Check();

        methods->Set(context, index, methodObject).Check();
    }

    jobjectArray fieldsValue = static_cast<jobjectArray>(env->GetObjectField(instance,
                                                                             fieldsField));
    int fieldLength = env->GetArrayLength(fieldsValue);
    v8::Local<v8::Array> fields = v8::Array::New(isolate, fieldLength);
    for (int index = 0; index < fieldLength; index++) {
        v8::Local<v8::Object> fieldObject = v8::Object::New(isolate);
        jobject fieldInstance = env->GetObjectArrayElement(
                fieldsValue,
                index);

        fieldObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "name"),
                         v8::String::NewFromUtf8(isolate, Util::JavaStr2CStr(
                                 static_cast<jstring>(env->CallObjectMethod(fieldInstance,
                                                                            getFieldName)))).ToLocalChecked()).Check();

        bool isMutable = !env->CallStaticBooleanMethod(modifierClass, isFinal,
                                                       env->CallIntMethod(fieldInstance,
                                                                          getModifiers));
        if (isMutable) {
            fieldObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "mutable"),
                             v8::Boolean::New(isolate,
                                              true)).Check();
        } else {
            fieldObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "mutable"),
                             v8::Boolean::New(isolate,
                                              false)).Check();
        }

        fields->Set(context, index, fieldObject).Check();
    }

    jobjectArray staticMethodsValue = static_cast<jobjectArray>(env->GetObjectField(instance,
                                                                                    staticMethodsField));
    int staticMethodLength = env->GetArrayLength(staticMethodsValue);
    v8::Local<v8::Array> staticMethods = v8::Array::New(isolate, staticMethodLength);
    for (int index = 0; index < staticMethodLength; index++) {
        v8::Local<v8::Object> staticMethodObject = v8::Object::New(isolate);
        jobject staticMethodInstance = env->GetObjectArrayElement(
                staticMethodsValue,
                index);

        staticMethodObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "name"),
                                v8::String::NewFromUtf8(isolate, Util::JavaStr2CStr(
                                        static_cast<jstring>(env->CallObjectMethod(
                                                staticMethodInstance,
                                                getMethodName)))).ToLocalChecked()).Check();

        staticMethods->Set(context, index, staticMethodObject).Check();
    }

    jobjectArray staticFieldsValue = static_cast<jobjectArray>(env->GetObjectField(instance,
                                                                                   staticFieldsField));
    int staticFieldLength = env->GetArrayLength(staticFieldsValue);
    v8::Local<v8::Array> staticFields = v8::Array::New(isolate, staticFieldLength);
    for (int index = 0; index < staticFieldLength; index++) {
        v8::Local<v8::Object> staticFieldObject = v8::Object::New(isolate);
        jobject staticFieldInstance = env->GetObjectArrayElement(
                staticFieldsValue,
                index);

        staticFieldObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "name"),
                               v8::String::NewFromUtf8(isolate, Util::JavaStr2CStr(
                                       static_cast<jstring>(env->CallObjectMethod(
                                               staticFieldInstance,
                                               getFieldName)))).ToLocalChecked()).Check();

        bool isMutable = !env->CallStaticBooleanMethod(modifierClass, isFinal,
                                                       env->CallIntMethod(staticFieldInstance,
                                                                          getModifiers));
        if (isMutable) {
            staticFieldObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "mutable"),
                                   v8::Boolean::New(isolate,
                                                    true)).Check();
        } else {
            staticFieldObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "mutable"),
                                   v8::Boolean::New(isolate,
                                                    false)).Check();
        }

        staticFields->Set(context, index, staticFieldObject).Check();
    }

    v8::Local<v8::Boolean> isArray;
    bool isArrayValue = env->GetBooleanField(instance, isArrayField);
    if (isArrayValue) {
        isArray = v8::Boolean::New(isolate, true);
    } else {
        isArray = v8::Boolean::New(isolate, false);
    }

    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "className"),
                   className).Check();
    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "superclass"),
                   superclass).Check();
    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "declaredClasses"),
                   declaredClasses).Check();
    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "methods"),
                   methods).Check();
    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "fields"),
                   fields).Check();
    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "staticMethods"),
                   staticMethods).Check();
    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "staticFields"),
                   staticFields).Check();
    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "isArray"),
                   isArray).Check();
    classInfo->Set(context, v8::String::NewFromUtf8Literal(isolate, "ref"),
                   v8::External::New(isolate, instance)).Check();
    return classInfo;
}
