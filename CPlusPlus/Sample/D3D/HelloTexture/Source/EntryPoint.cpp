#include "D3D.h"

#include <Windows.h>




Int32 EntryPoint()
{
	auto title = L"Hello Texture";

	HWND handle;
	SizeType width = 1280;
	SizeType height = 720;
	{
		UInt32 style = WS_OVERLAPPEDWINDOW;
		UInt32 extendStyle = 0;

		WNDCLASSEXW windowClass;
		Memory::Clear(&windowClass, sizeof(WNDCLASSEXW));
		windowClass.cbSize = sizeof(WNDCLASSEXW);
		windowClass.hInstance = GetModuleHandleW(nullptr);
		windowClass.lpfnWndProc = DefWindowProcW;
		windowClass.lpszClassName = L"Class Entry";
		RegisterClassExW(&windowClass);

		SizeType realWidth;
		SizeType realHeight;
		{
			RECT rect = { 0, 0, width, height };
			AdjustWindowRectEx(&rect, style, false, extendStyle);

			realWidth = rect.right - rect.left;
			realHeight = rect.bottom - rect.top;
		}

		handle = CreateWindowExW(
			extendStyle,
			windowClass.lpszClassName,
			title,
			style,
			CW_USEDEFAULT, CW_USEDEFAULT, realWidth, realHeight,
			nullptr, nullptr, windowClass.hInstance, nullptr
		);
		ShowWindow(handle, SW_SHOWDEFAULT);
	}

	D3DInit(handle, width, height);

	while (true)
	{
		MSG message;
		while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE) > 0)
			DispatchMessageW(&message);

		D3DUpdate();
		D3DRender();
	}

	D3DDestroy();

	return 0;
}


int main(int count, char** arguments)
{
	return EntryPoint();
}