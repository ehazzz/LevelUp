#pragma once

/**
* Windows OS Special API Set
* 
* 
* @remark
* Dont include this header file directly
*/



#if !defined(PLATFORM_WINDOWS)
#error This is not a Windows app
#endif


//#define WIN32_LEAN_AND_MEAN	// Exclude some stuff that rarely used from <Windows.h>
//#define NOMINMAX				// Undefine macros(min and max) from <minwindef.h>


#include <Windows.h>