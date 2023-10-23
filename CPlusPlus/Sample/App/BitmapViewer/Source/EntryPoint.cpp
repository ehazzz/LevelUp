#include "Application.h"



Int32 EntryPoint()
{
	Runtime runtime;
	{
		runtime.wantToQuit = false;
	}


	auto& eventSystem = runtime.eventSystem;

	auto& window = runtime.window;
	window.Create( L"Bitmap Viewer", 1600, 900, &eventSystem );

	GraphicDeviceInterface gdi( window.GetHandle() );

	OnAppInit( &runtime );

	// main loop
	while ( !runtime.wantToQuit )
	{
		static Timer	timer;
		static RealType	timestep;

		timestep = timer.Elapsed();
		timer.Reset();

		window.Update();
		for ( auto& event : eventSystem.eventQueue )
			switch ( event.type )
			{
			case Event::Type::WindowResize:
			{
				window.SetWidth( event.width );
				window.SetHeight( event.height );

				OnAppResize( event.width, event.height );
			}
				break;
			case Event::Type::WindowFocus:
				OnAppFocused( event.isFocused );
				break;
			case Event::Type::WindowClose:
				OnAppQuit( event.isClosed );
				break;

			case Event::Type::Key:
				eventSystem.keys[ static_cast<EnumUnderlyingType<Key>>(event.key) ] |= event.keyState;
				break;

			case Event::Type::MouseButton:
				eventSystem.buttons[ static_cast<EnumUnderlyingType<MouseButton>>(event.mouseButton) ] |= event.mouseButtonState;
				break;
			case Event::Type::MouseMove:
				eventSystem.mousePosition[0] = event.mouseX;
				eventSystem.mousePosition[1] = event.mouseY;
				break;
			case Event::Type::MouseScrollWheelScroll:
				eventSystem.mouseScrollWheelScrolledValue += event.mouseScrollWheelScrolledDelta;
				break;
			}

		OnUpdate(timestep);
		gdi.Present( window.GetWidth(), window.GetHeight(), OnRender );

		eventSystem.Reset();
	}

	return 0;
}


int main(int count, char** parameters)
{
	return EntryPoint();
}


#if defined(PLATFORM_WINDOWS)
#include <Windows.h>


int WINAPI wWinMain(HINSTANCE current, HINSTANCE previous, PWSTR commandLine, int commandShow)
{
	return EntryPoint();
}
#endif