#include "GraphicDeviceInterface.h"

#include "Windows.h"



static GraphicDeviceInterface Construct(Void* nativeWindowsWindowHandle)
{
	HDC targetDC = GetDC( CAST(HWND, nativeWindowsWindowHandle) );
	HDC memoryDC = CreateCompatibleDC(targetDC);

	GraphicDeviceInterface ret;
	ret.handle		= nativeWindowsWindowHandle;
	ret.targetDC	= targetDC;
	ret.memoryDC	= memoryDC;

	return ret;
}

static Void Destruct(GraphicDeviceInterface* instance)
{
	HWND handle		= CAST( HWND, instance->handle );
	HDC targetDC	= CAST( HDC, instance->targetDC );
	HDC memoryDC	= CAST( HDC, instance->memoryDC );

	if ( handle && targetDC )
		ReleaseDC(handle, targetDC);

	if (memoryDC)
		DeleteDC(memoryDC);
}

static Void Present(GraphicDeviceInterface* instance, SizeType width, SizeType height, FGraphicDeviceInterfaceRenderCallback Fn)
{
	HDC targetDC = CAST( HDC, instance->targetDC );
	HDC memoryDC = CAST( HDC, instance->memoryDC );

	BITMAPINFO bitmapInfo;
	FMemory.Clear( &bitmapInfo, sizeof(BITMAPINFO) );
	bitmapInfo.bmiHeader.biSize		= sizeof(BITMAPINFO);
	bitmapInfo.bmiHeader.biWidth	= width;
	bitmapInfo.bmiHeader.biHeight	= height;
	bitmapInfo.bmiHeader.biBitCount	= 8 * 4;
	bitmapInfo.bmiHeader.biPlanes	= 1;

	Void* RenderTarget	= NullPointer;
	HBITMAP hBitmap		= CreateDIBSection( memoryDC, &bitmapInfo, DIB_RGB_COLORS, &RenderTarget, NullPointer, 0 );

	if ( Fn && RenderTarget )
		Fn(RenderTarget);

	if (hBitmap)
	{
		SelectObject(memoryDC, hBitmap);
		BitBlt(targetDC, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);
		DeleteObject(hBitmap);
	}
}


struct FGraphicDeviceInterface FGraphicDeviceInterface =
{
	Construct,
	Destruct,
	Present,
};
