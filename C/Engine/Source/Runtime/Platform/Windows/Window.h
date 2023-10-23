#pragma once

#include "../../Core.h"

#include "../SAL/EventSystem.h"



typedef struct
{
	Void* handle;		// A native Windows window handle that will be used to indicate which window it is on desktop
	
	SizeType width;		// The width of client area of Windows window
	SizeType height;	// The height of client area of Windows window
}
Window;


struct FWindow
{
	// Create a Windows window and display it on desktop
	Window(*Construct)(const Char16* title, SizeType width, SizeType height, Void* userParameter);


	// Poll events from OS
	Void(*Update)(Window* instance);
};


extern ENGINE_API struct FWindow FWindow;