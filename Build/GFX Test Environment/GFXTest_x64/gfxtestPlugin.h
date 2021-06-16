/*
	GFXTest plugin SDK header (2014)

	learn_more
	evolution536

	This file is part of the GFXTest plugin SDK. Input and render plugins can be developed using the predefined
	functionality in this file. GFXTest can be found at unknowncheats.me and includes the SDK header.
	
	This SDK header file describes the interface definition of plugins in the TestPlugin folder. GFXTest looks for 
	DLL's in this folder upon application startup. GFXTest attempts to load all DLL's that have the 'pluginCallback'
	function exported. This function will be called for various events.
*/

#ifndef GFXTESTPLUGIN_H
#define GFXTESTPLUGIN_H

/*
	Plugin event enumeration. The meaning of the values are described below.
	
	Value						Argument Type		Argument Description
	
	pe_LoadRender				HMODULE				The render plugin that has just loaded.
	pe_UnloadRender				HMODULE				The render plugin before it is unloaded.
	pe_LoadInput				HMODULE				The input plugin that has just loaded.
	pe_UnloadInput				HMODULE				The input plugin before it is unloaded.
	pe_CreateWindow				HWND 				The handle to the main window after it is created.
	pe_DestroyWindow			HWND				The handle to the main window before its destroyed.
	pe_PreRender				HDC					A valid device context to use for the beginning of the drawing scene if it is not NULL.
	pe_PostRender				HDC					A valid device context to use for the ending of the drawing scene if it is not NULL.
	pe_MouseRel					int[2]				X and Y position of the mouse in relative movement.
	pe_MouseAbs					int[2]				X and Y position of the mouse in absolute position change.
	pe_MouseButton				int*				LOWORD(ptr) is a pointer to a MouseButtons_e structure, The indication bit for mouse down is at MOUSEBUTTON_DOWN.
	pe_Key						long*				The KEY_DOWN of this value is the indication bit for mouse down, the rest is the scan code.
*/

enum pluginEvent_e {
	pe_LoadRender		= 0,
	pe_UnloadRender		= 1,

	pe_LoadInput		= 10,
	pe_UnloadInput		= 11,

	pe_CreateWindow		= 20,
	pe_DestroyWindow	= 21,

	pe_PreRender		= 30,
	pe_PostRender		= 31,

	pe_MouseRel			= 40,
	pe_MouseAbs			= 41,
	pe_MouseButton		= 42,

	pe_Key				= 50,
};

#define MOUSEBUTTON_DOWN	0x10000
#define KEY_DOWN			0x00001

enum MouseButtons_e {
	mb_None,
	mb_Left,	// MOUSE1
	mb_Right,	// MOUSE2
	mb_Middle,	// MOUSE3
	mb_Button4, // MOUSE4
	mb_Button5	// MOUSE5
};

//export this function to be notified of events.
typedef void (__stdcall* pluginCallbackProc)( int pluginEvent, void* arg );

#define PLUGIN_PROCNAME		"pluginCallback"
#define PLUGIN_ALTPROCNAME	"_pluginCallback@8"

/*
	Action definitions to control the behavior of GFXTest from a loaded module. These may be called from any thread.
	
	Value						Argument Type		Argument Description						Return Value
	
	pa_LoadModule				wchar_t*			The filename of the module to load.			0 if the module was found. -1 if otherwise.
	pa_Reload					void*				NULL value is passed						NULL
	pa_Renderlist				wchar_t**			Should be passed a pointer to a wchar_t*	
													that points to a double null terminated
													string that lists the render plugins.
													GFXTest takes ownership of this array
													so DO NOT modify it.
	pa_Inputlist				wchar_t**			Should be passed a pointer to a wchar_t*	
													that points to a double null terminated
													string that lists the input plugins.
													GFXTest takes ownership of this array
													so DO NOT modify it.
	pa_Resolutionlist			wchar_t**			Should be passed a pointer to a wchar_t*	
													that points to a double null terminated
													string that lists the supported resolutions.
													GFXTest takes ownership of this array
													so DO NOT modify it.
*/

enum pluginAction_e {
	pa_LoadModule		= 0,
	pa_Reload			= 10,
	pa_Renderlist		= 20,
	pa_Inputlist		= 21,
	pa_Resolutionlist	= 22,
};

/*
	Function prototype and extern implementation for the TestControl function.
	This function is used by GFXTest to access loaded plugins and pass / retrieve
	the information as decribed above. It is advised to call the function using GetProcAddress.
*/

typedef int (__stdcall* testControlProc)(int, void*);

static int TestControl( int action, void* arg )
{
	static HMODULE hGFXTest = NULL;
	static testControlProc proc = NULL;
	if( !proc ) {
		hGFXTest = GetModuleHandle( NULL );
		proc = (testControlProc)GetProcAddress( hGFXTest, "_testControl@8" );
	}
	if( proc ) {
		return proc( action, arg );
	}
	return -1;
}

#endif //#ifndef GFXTESTPLUGIN_H