package com.mucheng.nodejava.javabridge;

final class JavaBridgeUtil {

    private static boolean unsafeReflectionEnabled;

    private JavaBridgeUtil() {
    }

    public static ClassInfo findClassOrNull(String className) {
        ClassLoader classLoader = getClassLoader();
        try {
            return new ClassInfo(classLoader.loadClass(className));
        } catch (ClassNotFoundException e) {
            return null;
        }
    }

    private static ClassLoader getClassLoader() {
        return JavaBridgeUtil.class.getClassLoader();
    }

    public static boolean isUnsafeReflectionEnabled() {
        return unsafeReflectionEnabled;
    }

}
