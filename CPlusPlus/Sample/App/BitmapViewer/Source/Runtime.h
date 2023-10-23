#pragma once

#include "Engine.h"



using namespace Engine;
using namespace Engine::Windows;


struct Runtime
{
	Window		window;			// Main window of the process(app)
	EventSystem eventSystem;	// Used to trace some status for user interaction

	Bool wantToQuit; // A flag to confirm if the app should be exited
	Bool isFocused;
};