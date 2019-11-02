/**
 * @author Nathan Sweet <misc at n4te.com>
 */

// Disable STL warning.
#pragma warning(disable:4786)

#include <jni.h>
#include "jniutil.h"
#include <locale.h>

// Index to stored JNIEnvs per thread.
static DWORD tlsIndex = 0;

// Captures and releases resources for the DLL.
static BOOL InitializeResources (HINSTANCE dll);
static void UninitializeResources ();

// Stores and releases data for a JNIEnv for a TLS bucket.
static void InitializeEnv (JNIEnv *env);
static void UninitializeEnv (JNIEnv* env);

// Called when an JNIEnv should be stored for a thread if it isn't already.
static void StoreEnv (JNIEnv *env) {
	if (tlsIndex == 0) {
		tlsIndex = TlsAlloc();
		if (tlsIndex == -1) {
			throwRuntimeException(env, "Unable to allocate Thread Local Storage index.");
			return;
		}
	}
	::TlsSetValue(tlsIndex, env);
	::InitializeEnv(env);
}

// DLL entry point.
BOOL WINAPI DllMain (HINSTANCE dll, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		if (!::InitializeResources(dll)) return FALSE;
		break;
	case DLL_PROCESS_DETACH:
		::UninitializeResources();
		break;
	}
	return TRUE;
}
