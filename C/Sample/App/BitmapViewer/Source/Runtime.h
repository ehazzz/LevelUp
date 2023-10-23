#pragma once

#include "Engine.h"



typedef struct
{
	Window		window;			// Main window of the process(app)
	EventSystem	eventSystem;	// Used to trace some statuses for user intraction

	Bool wantToQuit; // A flag to confirm if this app should be exited
	Bool isFocused;
}
Runtime;