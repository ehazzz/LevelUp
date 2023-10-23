#include "Window.h"

#include "Windows.h"



namespace Engine::Windows
{
	static LRESULT WINAPI WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static Void* userParameter = nullptr;


		EventSystem* eventSystem = reinterpret_cast<EventSystem*>(userParameter);
		switch (message)
		{
		case WM_CREATE:
			userParameter = reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams;
			break;
		case WM_SIZE:
		{
			SizeType width	= ( lParam & 0x0000FFFF ) >>  0;
			SizeType height = ( lParam & 0xFFFF0000 ) >> 16;

			eventSystem->AddWindowResizeEvent(width, height);
		}
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			eventSystem->AddWindowFocusEvent( message == WM_SETFOCUS );
			break;
		case WM_CLOSE:
		case WM_DESTROY:
			eventSystem->AddWindowCloseEvent(true);
			break;

		case WM_KEYDOWN:
		case WM_KEYUP:
			eventSystem->AddKeyEvent(
				static_cast<Key>(wParam),
				message == WM_KEYDOWN ? KeyState::Press : KeyState::Release
			);
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

			// The last mouse button had been released if isZero
			Bool isZero = wParam == 0 ? true : false;

			eventSystem->AddMouseButtonEvent(
				isZero ? static_cast<MouseButton>(lastWParam) : static_cast<MouseButton>(wParam),
				isZero ? MouseButtonState::Release : MouseButtonState::Press
			);

			lastWParam = wParam;
		}
			break;
		case WM_MOUSEMOVE:
		{
			SizeType clientHeight;
			{
				RECT rect;
				GetClientRect( handle, &rect );

				clientHeight = rect.bottom - rect.top;
			}

			Int32 x = ( lParam & 0x0000FFFF ) >>  0;
			Int32 y = ( lParam & 0xFFFF0000 ) >> 16;

			eventSystem->AddMouseMoveEvent( x, clientHeight - 1 - y );
		}
			break;
		case WM_MOUSEWHEEL:
			eventSystem->AddMouseScrollWheelScrollEvent(
				GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA
			);
			break;

		default:
			return DefWindowProcW(handle, message, wParam, lParam);
		}

		return LRESULT(1);
	}


	Window::Window()
	{
		Memory::Clear( this, sizeof(Window) );
	}

	Window::~Window()
	{

	}

	Void Window::Create(const Char16* title, SizeType width, SizeType height, Void* userParameter)
	{
		auto instance	= GetModuleHandleW(nullptr);
		auto className	= L"Class Entry";


		// window style
		UInt32 style		= WS_OVERLAPPEDWINDOW;
		UInt32 extendStyle	= 0;

		// register window class
		{
			WNDCLASSEXW descriptor;
			Memory::Clear( &descriptor, sizeof(WNDCLASSEXW) );
			descriptor.cbSize			= sizeof(WNDCLASSEXW);
			descriptor.hInstance		= instance;
			descriptor.lpfnWndProc		= WindowProcedure;
			descriptor.lpszClassName	= className;

			RegisterClassExW( &descriptor );
		}

		// resized to expected size
		SizeType realWidth;
		SizeType realHeight;
		{
			RECT rect = { 0, 0, width, height };
			AdjustWindowRectEx( &rect, style, false, extendStyle );

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
			nullptr, nullptr, instance, userParameter
		);
		ShowWindow(handle, SW_SHOWDEFAULT);


		this->handle	= handle;
		this->width		= width;
		this->height	= height;
	}

	Void Window::Update()
	{
		MSG message;
		while ( PeekMessageW( &message, nullptr, 0, 0, PM_REMOVE ) > 0 )
			DispatchMessageW( &message );
	}

	Void Window::SetTitle(const Char16* title)
	{
		SetWindowTextW( reinterpret_cast<HWND>(handle), title );
	}

	Void Window::SetIsResizableWindow(Bool isResizable)
	{
		auto handle = reinterpret_cast<HWND>( this->handle );


		auto previous = GetWindowLongPtrW(handle, GWL_STYLE);

		LONG_PTR current;
		if (isResizable)
			current = previous | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		else
			current = previous & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX;
		
		SetWindowLongPtrW(handle, GWL_STYLE, current);
	}

	Void Window::SetWidth(SizeType width)
	{
		this->width = width;
	}

	Void Window::SetHeight(SizeType height)
	{
		this->height = height;
	}

	Void* Window::GetHandle() const
	{
		return handle;
	}

	SizeType Window::GetWidth() const
	{
		return width;
	}

	SizeType Window::GetHeight() const
	{
		return height;
	}
}