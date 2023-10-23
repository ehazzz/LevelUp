#pragma once

/**
* Used By The Other C Or C++ Project
* 
* 
* @remark
* Build this project as a dynamic link library
*/



/**
* [Include]
* 
* Path(Visual Studio Macro): $(SolutionDir)C/Engine/Source/Include/
*/


/**
* [Link]
* 
* Path(Visual Studio Macro): $(SolutionDir)Binary/C/$(Configuration)/$(Platform)/
*/
#pragma comment(lib, "Engine")


/**
* For Visual Studio
* 
* Output Directory:			$(SolutionDir)Binary/C/$(Configuration)/$(Platform)/
* Intermediate Directory:	$(SolutionDir)Binary/C/$(Configuration)/$(Platform)/$(ProjectName)/
*/


/**
* Import engine
* 
* 
* @remark
* As a flag to define ENGINE_API as DLL_IMPORT
*/
#if !defined(ENGINE_API_IMPORTS)
#define ENGINE_API_IMPORTS
#endif


/** API set */
#include "../Runtime/Core.h"

#include "../Runtime/Asset/Bitmap.h"
#include "../Runtime/Asset/Wave.h"

#include "../Runtime/Platform/SAL/Directory.h"
#include "../Runtime/Platform/SAL/EventSystem.h"
#include "../Runtime/Platform/SAL/File.h"
#include "../Runtime/Platform/SAL/Path.h"

#if defined(PLATFORM_WINDOWS)
#include "../Runtime/Platform/Windows/DirectSound.h"
#include "../Runtime/Platform/Windows/FileDialog.h"
#include "../Runtime/Platform/Windows/GraphicDeviceInterface.h"
#include "../Runtime/Platform/Windows/Window.h"
#endif