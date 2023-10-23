#pragma once

/**
* Windows GDI Wrapper
*/

#include "../../Core.h"



typedef struct
{
	Void* handle;	// Native Windows window handle
	Void* targetDC; // Target device context
	Void* memoryDC; // Memory device context
}
GraphicDeviceInterface;


typedef Void(*FGraphicDeviceInterfaceRenderCallback)(Void* RenderTarget);


struct FGraphicDeviceInterface
{
	GraphicDeviceInterface(*Construct)(Void* nativeWindowsWindowHandle);
	Void(*Destruct)(GraphicDeviceInterface* instance);


	/**
	* Overwrite RenderTarget by a function pointer(Fn) to update the client area of Windows window 
	* 
	* 
	* @remark
	* A pointer(RenderTarget) point to heap memory's size in bytes defined by width * height * 4
	* RenderTarget's pixel format is B8G8R8A8(a pixel's color uses 32 bits to represent)
	* 
	* Memory Layout(B8G8R8A8):
	* |<- blue ->||<- green->||<- red  ->||<- alpha->|
	* [0b00000000][0b00000000][0b00000000][0b00000000]
	* 0          7
	*             8          15
	*                         16         23
	*                                     24         31
	*/
	Void(*Present)(GraphicDeviceInterface* instance, SizeType width, SizeType height, FGraphicDeviceInterfaceRenderCallback Fn);
};


extern ENGINE_API struct FGraphicDeviceInterface FGraphicDeviceInterface;