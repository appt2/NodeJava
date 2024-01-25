package com.mucheng.nodejava.javabridge;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.List;

public class ClassInfo {

    public String className;

    public String superclass;

    public String[] declaredClasses;

    public Method[] methods;

    public Field[] fields;

    public Method[] staticMethods;

    public Field[] staticFields;

    public boolean isArray;

    public ClassInfo(Class<?> clazz) {
        this.className = clazz.getName();

        Class<?> superclass = clazz.getSuperclass();
        if (superclass != null) {
            this.superclass = superclass.getName();
        }

        Class<?>[] declaredClasses = clazz.getDeclaredClasses();
        this.declaredClasses = new String[declaredClasses.length];

        for (int index = 0; index < declaredClasses.length; index++) {
            this.declaredClasses[index] = declaredClasses[index].getName();
        }

        Method[] allMethods = JavaBridgeUtil.isUnsafeReflectionEnabled() ? clazz.getDeclaredMethods() : clazz.getMethods();
        Field[] allFields = JavaBridgeUtil.isUnsafeReflectionEnabled() ? clazz.getDeclaredFields() : clazz.getFields();

        List<Method> theMethods = new ArrayList<>();
        List<Field> theFields = new ArrayList<>();
        List<Method> theStaticMethods = new ArrayList<>();
        List<Field> theStaticFields = new ArrayList<>();

        for (Method theMethod : allMethods) {
            if (Modifier.isStatic(theMethod.getModifiers())) {
                theStaticMethods.add(theMethod);
            } else {
                theMethods.add(theMethod);
            }
        }

        for (Field theField : allFields) {
            if (Modifier.isStatic(theField.getModifiers())) {
                theStaticFields.add(theField);
            } else {
                theFields.add(theField);
            }

        }

        Method[] methodTypedArray = new Method[0];
        Field[] fieldTypedArray = new Field[0];

        this.methods = theMethods.toArray(methodTypedArray);
        this.fields = theFields.toArray(fieldTypedArray);
        this.staticMethods = theStaticMethods.toArray(methodTypedArray);
        this.staticFields = theStaticFields.toArray(fieldTypedArray);

        this.isArray = clazz.isArray();
    }

}
