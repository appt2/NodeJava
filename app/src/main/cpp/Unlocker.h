//
// Created by 35785 on 2024/1/25.
//

#ifndef NODEJAVA_UNLOCKER_H
#define NODEJAVA_UNLOCKER_H
#include <jni.h>
#include <v8.h>
#include "Isolate.h"

class Unlocker {
public:
    Isolate *isolate;
    v8::Unlocker *self;

    Unlocker(Isolate *isolate);

    static void To(jobject instance, Unlocker *unlocker);

    static Unlocker *From(jobject instance);


};


#endif //NODEJAVA_UNLOCKER_H
