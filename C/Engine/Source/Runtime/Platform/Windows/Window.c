#include "Window.h"

#include "Windows.h"



static LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Void* userParameter;


	EventSystem* eventSystem = CAST(EventSystem*, userParameter);
	switch (message)
	{
	case WM_CREATE:
		userParameter = CAST(LPCREATESTRUCTW, lParam)->lpCreateParams;
		break;

	case WM_SIZE:
	{
		SizeType width	= ( lParam & 0x0000FFFF ) >>  0;
		SizeType height	= ( lParam & 0xFFFF0000 ) >> 16;

		FEventSystem.AddWindowResizeEvent(eventSystem, width, height);
	}
		break;
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		FEventSystem.AddWindowFocusEvent( eventSystem, message == WM_SETFOCUS );
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		FEventSystem.AddWindowCloseEvent(eventSystem, True);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		FEventSystem.AddKeyEvent( eventSystem, wParam, message == WM_KEYDOWN ? KeyState_Press : KeyState_Release );
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		// cache the index of the last mouse button
		static WPARAM lastWParam;

		// The last mouse mouse button had been released if isZero
		Bool isZero = wParam == 0 ? True : False;

		FEventSystem.AddKeyEvent(
			eventSystem,
			isZero ? lastWParam : wParam,
			isZero ? MouseButtonState_Release : MouseButtonState_Press
		);

		lastWParam = wParam;
	}
		break;
	case WM_MOUSEMOVE:
	{
		SizeType clientAreaHeight;
		{
			RECT rect;
			GetClientRect( handle, &rect );

			clientAreaHeight = rect.bottom - rect.top;
		}

		Int32 x = ( lParam & 0x0000FFFF ) >>  0;
		Int32 y = ( lParam & 0xFFFF0000 ) >> 16;

		FEventSystem.AddMouseMoveEvent( eventSystem, x, clientAreaHeight - 1 - y );
	}
	case WM_MOUSEWHEEL:
		FEventSystem.AddMouseScrollWheelScrollEvent( eventSystem, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA );
		break;

	default:
		return DefWindowProcW(handle, message, wParam, lParam);
	}

	return True;
}


static Window Construct(const Char16* title, SizeType width, SizeType height, Void* userParameter)
{
	HINSTANCE hInstance		= GetModuleHandleW(NullPointer);
	const Char16* className = L"Class Entry";


	// style
	UInt32 style		= WS_OVERLAPPEDWINDOW;
	UInt32 extendStyle	= 0;

	// register window class
	{
		WNDCLASSEXW windowClass;
		FMemory.Clear( &windowClass, sizeof(WNDCLASSEXW) );
		windowClass.cbSize			= sizeof(WNDCLASSEXW);
		windowClass.lpfnWndProc		= WindowProcedure;
		windowClass.hInstance		= hInstance;
		windowClass.lpszClassName	= className;

		RegisterClassExW( &windowClass );
	}

	// resized to expected size
	SizeType realWidth;
	SizeType realHeight;
	{
		RECT rect = { 0, 0, width, height };
		AdjustWindowRectEx( &rect, style, False, extendStyle );

		realWidth	= rect.right - rect.left;
		realHeight	= rect.bottom - rect.top;
	}

	// create and display
	HWND handle = CreateWindowExW(
		extendStyle,
		className,
		title,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, realWidth, realHeight,
		NullPointer, NullPointer, hInstance, userParameter
	);
	ShowWindow(handle, SW_SHOWDEFAULT);


	Window ret;
	ret.handle		= handle;
	ret.width		= width;
	ret.height		= height;

	return ret;
}

static Void Update(Window* instance)
{
	MSG message;
	while ( PeekMessageW( &message, CAST( HWND, instance->handle ), 0, 0, PM_REMOVE ) )
	{
		DispatchMessageW( &message );
	}
}


struct FWindow FWindow = 
{
	Construct,
	Update,
};
