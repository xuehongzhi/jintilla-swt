/**
 * @author Nathan Sweet <misc at n4te.com>
 */

static jint getIntValue (JNIEnv* env, jobject obj, const char* name) {
	jclass clazz = env->GetObjectClass(obj);
	jfieldID id = env->GetFieldID(clazz, name, "I");
	return env->GetIntField(obj, id);
}

static void setIntValue (JNIEnv* env, jobject obj, const char* name, jint value) {
	jclass clazz = env->GetObjectClass(obj);
	jfieldID id = env->GetFieldID(clazz, name, "I");
	env->SetIntField(obj, id, value);
}

static jstring getStrValue (JNIEnv* env, jobject obj, const char* name) {
	jclass clazz = env->GetObjectClass(obj);
	jfieldID id = env->GetFieldID(clazz, name, "Ljava/lang/String;");
	return (jstring)env->GetObjectField(obj, id);
}

static void setStrValue (JNIEnv* env, jobject obj, const char* name, jstring value) {
	jclass clazz = env->GetObjectClass(obj);
	jfieldID id = env->GetFieldID(clazz, name, "Ljava/lang/String;");
	env->SetObjectField(obj, id, value);
}

static void throwRuntimeException (JNIEnv* env, const char* message) {
	jclass runtimeExceptionClass = env->FindClass("java/lang/RuntimeException");
	if (runtimeExceptionClass) {
		env->ThrowNew(runtimeExceptionClass, message);
	} else {
		printf("\nJNI error: Unable to throw RuntimeException.\n%s\n\n", message); fflush(stdout);
	}
}
