//
// Created by 35785 on 2024/1/25.
//

#ifndef NODEJAVA_LOCKER_H
#define NODEJAVA_LOCKER_H

#include <jni.h>
#include <v8.h>
#include "Isolate.h"

class Locker {
public:
    Isolate *isolate;
    v8::Locker *self;

    Locker(Isolate *isolate);

    static void To(jobject instance, Locker *self);

    static Locker *From(jobject instance);

    void release();

    static bool isLocked(Isolate *isolate);
};


#endif //NODEJAVA_LOCKER_H
