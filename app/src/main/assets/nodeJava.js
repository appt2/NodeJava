(function() {
    const $java = process._linkedBinding("java");
    $java.constructors = {};
    $java.prototypes = {};

    $java.findClassOrNull = function (className) {
        const cachedClass = $java.constructors[className];
        if (typeof cachedClass !== "undefined") {
            return cachedClass;
        }

        const classInfo = $java.getClassInfo(className);
        if (!classInfo) {
            return null;
        }

        return classInfo;
    }

    globalThis["$java"] = $java;
})();

$java.setUnsafeReflectionEnabled(true);

const clazz = $java.findClassOrNull("com.mucheng.nodejava.core.Context");
console.log(clazz);