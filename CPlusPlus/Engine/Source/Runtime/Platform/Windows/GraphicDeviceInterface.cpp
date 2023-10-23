#include "GraphicDeviceInterface.h"

#include "Windows.h"



namespace Engine::Windows
{
	GraphicDeviceInterface::GraphicDeviceInterface(Void* nativeWindowsWindowHandle)
	{
		HDC targetDC = GetDC( reinterpret_cast<HWND>(nativeWindowsWindowHandle) );
		HDC memoryDC = CreateCompatibleDC(targetDC);

		this->handle	= nativeWindowsWindowHandle;
		this->targetDC	= targetDC;
		this->memoryDC	= memoryDC;
	}

	GraphicDeviceInterface::~GraphicDeviceInterface()
	{
		HWND handle		= reinterpret_cast<HWND>( this->handle );
		HDC targetDC	= reinterpret_cast<HDC>( this->targetDC );
		HDC memoryDC	= reinterpret_cast<HDC>( this->memoryDC );

		if ( handle && targetDC )
			ReleaseDC(handle, targetDC);

		if (memoryDC)
			DeleteDC(memoryDC);
	}

	Void GraphicDeviceInterface::Present(SizeType width, SizeType height, RenderCallback Fn)
	{
		HDC targetDC = reinterpret_cast<HDC>( this->targetDC );
		HDC memoryDC = reinterpret_cast<HDC>( this->memoryDC );

		BITMAPINFO descriptor;
		Memory::Clear( &descriptor, sizeof(BITMAPINFO) );
		descriptor.bmiHeader.biSize		= sizeof(BITMAPINFO);
		descriptor.bmiHeader.biWidth	= width;
		descriptor.bmiHeader.biHeight	= height;
		descriptor.bmiHeader.biBitCount	= 8 * 4;
		descriptor.bmiHeader.biPlanes	= 1;

		Void* RenderTarget	= nullptr;
		HBITMAP hBitmap		= CreateDIBSection( memoryDC, &descriptor, DIB_RGB_COLORS, &RenderTarget, nullptr, 0 );

		if ( RenderTarget && Fn )
			Fn(RenderTarget);

		if (hBitmap)
		{
			SelectObject(memoryDC, hBitmap);
			BitBlt(targetDC, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);
			DeleteObject(hBitmap);
		}
	}
}