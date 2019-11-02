/**
 * @author Nathan Sweet <misc at n4te.com>
 */

#include <map>

// Stores control scoped data.
struct ControlState;

// Looks up a ControlState from an SWT control handle.
static std::map<HWND, ControlState*> controlToState;

// Called to handle a windows message.
static void HandleMessage(ControlState* controlState, JNIEnv* env, HWND hwndControl, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc (HWND hwndControl, UINT msg, WPARAM wParam, LPARAM lParam) {
	ControlState* controlState = controlToState[hwndControl];
	JNIEnv* env = (JNIEnv*)::TlsGetValue(tlsIndex);
	if (controlState == NULL || env == NULL) return FALSE;

	// If an exception has already occurred, allow the stack to unwind so that the exception will be thrown in Java.
	if (env->ExceptionOccurred()) return FALSE;

	::HandleMessage(controlState, env, hwndControl, msg, wParam, lParam);

	return ::CallWindowProc(controlState->oldProc, hwndControl, msg, wParam, lParam);
}

static ControlState* GetControlState (jint hwndControl) {
	// Create.
	ControlState* controlState = (ControlState*)::LocalAlloc(LPTR, sizeof(ControlState));
	if (controlState == 0) return NULL;

	// Populate.
	controlState->hwndControl = (HWND)hwndControl;
	controlState->oldProc = (WNDPROC)::GetWindowLong((HWND)hwndControl, GWL_WNDPROC);
	::SetWindowLong((HWND)hwndControl, GWL_WNDPROC, (long)WindowProc);

	// Store.
	std::pair<HWND,ControlState*> controlToStatePair((HWND)hwndControl, controlState);
	controlToState.insert(controlToStatePair);

	return controlState;
}

static void disposeControlState (jint hwndControl) {
	ControlState* controlState = controlToState[(HWND)hwndControl];
	controlToState.erase((HWND)hwndControl);
	::LocalFree(controlState);
}
