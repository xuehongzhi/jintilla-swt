/**
 * @author Valentin Valchev <v_valchev at prosyst.bg>
 * @author Nathan Sweet <misc at n4te.com>
 */

#include "SciJNI.h"
#include "Scintilla.h"  // Get Scintilla.h from http://www.scintilla.org/
#include <windows.h>

struct ControlState {
	WNDPROC oldProc;
	HWND hwndControl;
	// Control scoped data.
	HWND hwndScintilla;
	int (*messageFunction)(void*, int, int, int);
	void* messagePointer;
};

#include "jnidll.h"
#include "jnicontrol.h"

static jclass scintillaClass;
static jmethodID dispatchNotificationEventMethodID;
static jclass notificationEventClass;
static jmethodID notificationEventConstructorID;

static HINSTANCE sciJniDLL;
static HINSTANCE sciLexerDLL;

static BOOL InitializeResources (HINSTANCE dll) {
	sciJniDLL = dll;
	sciLexerDLL = LoadLibrary("sciLexer.DLL");
	if (sciLexerDLL == NULL) return FALSE;
	return TRUE;
}

static void UninitializeResources () {
	FreeLibrary(sciLexerDLL);
}

static void InitializeEnv (JNIEnv *env) {
	jclass clazz = env->FindClass("org/scintilla/editor/Scintilla");
	if ( !clazz ) {
		throwRuntimeException(env, "JNI error: Class not found: org.scintilla.editor.Scintilla");
		return;
	}
	scintillaClass = (jclass)env->NewGlobalRef((jobject)clazz);
	dispatchNotificationEventMethodID = env->GetStaticMethodID(scintillaClass, "dispatchNotificationEvent", "(ILorg/scintilla/editor/NotificationEvent;)V");

	clazz = env->FindClass("org/scintilla/editor/NotificationEvent");
	if ( !clazz ) {
		throwRuntimeException(env, "JNI error: Class not found: org.scintilla.editor.NotificationEvent");
		return;
	}
	notificationEventClass = (jclass)env->NewGlobalRef((jobject)clazz);
	notificationEventConstructorID = env->GetMethodID(notificationEventClass, "<init>", "()V");
}

static void UninitializeEnv (JNIEnv* env) {
	env->DeleteGlobalRef(scintillaClass);
	env->DeleteGlobalRef(notificationEventClass);
}

static void HandleMessage(ControlState* controlState, JNIEnv* env, HWND hwndControl, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg != WM_NOTIFY) return;

	jobject notificationParam = env->NewObject(notificationEventClass, notificationEventConstructorID);

	NMHDR *nmhdr = (NMHDR*)lParam;
	setIntValue( env, notificationParam, "hwndFrom", (jint)nmhdr->hwndFrom );
	setIntValue( env, notificationParam, "idFrom", nmhdr->idFrom );
	setIntValue( env, notificationParam, "code", nmhdr->code );

	SCNotification* notification = (SCNotification*)lParam;
	switch ( nmhdr->code ) {
	case SCN_CALLTIPCLICK:
	case SCN_STYLENEEDED:
		setIntValue( env, notificationParam, "position", notification->position );
		break;
	case SCN_CHARADDED:
		setIntValue( env, notificationParam, "ch", notification->ch );
		break;
	case SCN_KEY:
		setIntValue( env, notificationParam, "ch", notification->ch );
		setIntValue( env, notificationParam, "modifiers", notification->modifiers );
		break;
	case SCN_MODIFIED:
		setIntValue( env, notificationParam, "position", notification->position );
		setIntValue( env, notificationParam, "modificationType", notification->modificationType );
		if ( notification->text != 0 ) {
			// Make notification->text be null terminated.
			char* text = (char*)malloc( notification->length + 1 );
			strncpy( text, notification->text, notification->length );
			text[ notification->length ] = '\0';
			setStrValue( env, notificationParam, "text", env->NewStringUTF(text) );
		}
		setIntValue( env, notificationParam, "length", notification->length );
		setIntValue( env, notificationParam, "linesAdded", notification->linesAdded );
		setIntValue( env, notificationParam, "line", notification->line );
		setIntValue( env, notificationParam, "foldLevelNow", notification->foldLevelNow );
		setIntValue( env, notificationParam, "foldLevelPrev", notification->foldLevelPrev );
		break;
	case SCN_MACRORECORD:
		setIntValue( env, notificationParam, "message", notification->message );
		setIntValue( env, notificationParam, "wParam", notification->wParam );
		setIntValue( env, notificationParam, "lParam", notification->lParam );
		break;
	case SCN_MARGINCLICK:
		setIntValue( env, notificationParam, "margin", notification->margin );
		break;
	case SCN_USERLISTSELECTION:
		setIntValue( env, notificationParam, "listType", notification->listType );
		break;
	case SCN_DWELLEND:
	case SCN_DWELLSTART:
		setIntValue( env, notificationParam, "position", notification->position );
		setIntValue( env, notificationParam, "x", notification->x );
		setIntValue( env, notificationParam, "y", notification->y );
		break;
	case SCN_HOTSPOTCLICK:
	case SCN_HOTSPOTDOUBLECLICK:
		setIntValue( env, notificationParam, "position", notification->position );
		setIntValue( env, notificationParam, "modifiers", notification->modifiers );
		break;
	}

	env->CallStaticVoidMethod(
		scintillaClass, dispatchNotificationEventMethodID,
		nmhdr->hwndFrom, notificationParam
	);
}

JNIEXPORT jint JNICALL Java_org_scintilla_editor_Scintilla_createControl
  ( JNIEnv *env, jclass that, jint hwndControl )
{
	HWND hwndScintilla = CreateWindow(
		"Scintilla", "", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		0, 0, 0, 0, (HWND)hwndControl, NULL, sciJniDLL, NULL
	);
	if (hwndScintilla == 0) return (jint)0;

	::StoreEnv(env);

	ControlState* controlState = GetControlState(hwndControl);
	if (controlState == NULL) return (jint)0;
	controlState->hwndScintilla = hwndScintilla;
	controlState->messageFunction = (int (__cdecl*)(void*, int, int, int))::SendMessage( hwndScintilla, SCI_GETDIRECTFUNCTION, 0, 0 );
	controlState->messagePointer = (void*)::SendMessage( hwndScintilla, SCI_GETDIRECTPOINTER, 0, 0 );

	return (jint)hwndScintilla;
}

JNIEXPORT void JNICALL Java_org_scintilla_editor_Scintilla_resizeControl
  ( JNIEnv *env, jclass that, jint hwndScintilla, jint x, jint y, jint width, jint height )
{
	::SetWindowPos( (HWND)hwndScintilla, (HWND)0, x, y, width, height, SWP_NOZORDER|SWP_DRAWFRAME|SWP_NOACTIVATE );
}

JNIEXPORT void JNICALL Java_org_scintilla_editor_Scintilla_setFocus
  ( JNIEnv *env, jclass that, jint hwndScintilla )
{
    ::SetFocus( (HWND)hwndScintilla );
}

#define __SENDMESSAGE Java_org_scintilla_editor_Scintilla_sendMessage__III

JNIEXPORT jint JNICALL Java_org_scintilla_editor_Scintilla_sendMessage__III
  ( JNIEnv* env, jobject scintillaObject, jint msg, jint wparam, jint lparam )
{
	jint hwndControl = getIntValue( env, scintillaObject, "handle" );
	if ( hwndControl != 0 ) {
		ControlState* controlState = controlToState[(HWND)hwndControl];
		return controlState->messageFunction(controlState->messagePointer, msg, wparam, lparam);
	} else
		return 0;
}

JNIEXPORT jint JNICALL Java_org_scintilla_editor_Scintilla_sendMessage__IILjava_lang_String_2
  ( JNIEnv* env, jobject scintillaObject, jint msg, jint wparam, jstring lparam )
{
	const char* _lparam = env->GetStringUTFChars( lparam, NULL );

	jint ret = __SENDMESSAGE( env, scintillaObject, msg, wparam, (jint)_lparam );

	env->ReleaseStringUTFChars( lparam, _lparam );
	return ret;
}

JNIEXPORT jint JNICALL Java_org_scintilla_editor_Scintilla_sendMessage__ILjava_lang_String_2Ljava_lang_String_2
  ( JNIEnv* env, jobject scintillaObject, jint msg, jstring wparam, jstring lparam )
{
	const char* _wparam = env->GetStringUTFChars( wparam, NULL );
	const char* _lparam = env->GetStringUTFChars( lparam, NULL );

	jint ret = __SENDMESSAGE( env, scintillaObject, msg, (jint)_wparam, (jint)_lparam );

	env->ReleaseStringUTFChars( wparam, _wparam );
	env->ReleaseStringUTFChars( lparam, _lparam );

	return ret;
}

JNIEXPORT jint JNICALL Java_org_scintilla_editor_Scintilla_sendMessage__IILorg_scintilla_editor_CText_2
  ( JNIEnv* env, jobject scintillaObject, jint msg, jint wparam, jobject javaCText )
{
	jint capacity = getIntValue( env, javaCText, "capacity" );
	char* text = (char*)malloc( sizeof(char) * capacity );

	jint ret = __SENDMESSAGE( env, scintillaObject, msg, wparam, (long)text );

	// BOZO - May need to use the return value to null terminate the string for some methods.
	setStrValue( env, javaCText, "text", env->NewStringUTF(text) );

	free( text );

	return ret;
}

JNIEXPORT jint JNICALL Java_org_scintilla_editor_Scintilla_sendMessage__IILorg_scintilla_editor_TextRange_2
  ( JNIEnv* env, jobject scintillaObject, jint msg, jint wparam, jobject javaTextRange )
{
	struct TextRange textRange;

	struct CharacterRange chrg;
	chrg.cpMin = getIntValue( env, javaTextRange, "start" );
	chrg.cpMax = getIntValue( env, javaTextRange, "end" );
	textRange.chrg = chrg;

	long length;
	if ( chrg.cpMax == -1 )  // Select to end of document. capacity must be set.
		length = getIntValue( env, javaTextRange, "capacity" ) + 1;  // Add one for string terminating 0.
	else
		length = ( chrg.cpMax - chrg.cpMin ) + 1;  // Add one for string terminating 0.

	char* lpstrText = (char*)malloc( sizeof(char) * length );
	textRange.lpstrText = lpstrText;

	jint ret = __SENDMESSAGE( env, scintillaObject, msg, wparam, (long)&textRange );

	setStrValue( env, javaTextRange, "text", env->NewStringUTF(lpstrText) );

	free( lpstrText );

	return ret;
}

JNIEXPORT jint JNICALL Java_org_scintilla_editor_Scintilla_sendMessage__IILorg_scintilla_editor_FindText_2
  ( JNIEnv* env, jobject scintillaObject, jint msg, jint wparam, jobject javaFindText )
{
	struct TextToFind findText;

	struct CharacterRange chrg;
	chrg.cpMin = getIntValue( env, javaFindText, "start" );
	chrg.cpMax = getIntValue( env, javaFindText, "end" );
	findText.chrg = chrg;

	struct CharacterRange chrgText;
	chrgText.cpMin = -1;
	chrgText.cpMax = -1;
	findText.chrgText = chrgText;

	jstring searchPattern = getStrValue( env, javaFindText, "searchPattern" );
	const char* constText = env->GetStringUTFChars( searchPattern, NULL );
	char* lpstrText = (char*)malloc( strlen(constText) + 1 );  // Add one for string terminating 0.
	strcpy( lpstrText, constText );
	findText.lpstrText = lpstrText;

	jint ret = __SENDMESSAGE( env, scintillaObject, msg, wparam, (long)&findText );

	setIntValue( env, javaFindText, "startFound", findText.chrgText.cpMin );
	setIntValue( env, javaFindText, "endFound", findText.chrgText.cpMax );

	env->ReleaseStringUTFChars( searchPattern, constText );
	free( lpstrText );

	return ret;
}

JNIEXPORT void JNICALL Java_org_scintilla_editor_Scintilla_disposeControl
  (JNIEnv *env, jclass that, jint hwndControl)
{
	ControlState* controlState = controlToState[(HWND)hwndControl];
	controlToState.erase((HWND)hwndControl);
	::LocalFree(controlState);
}
