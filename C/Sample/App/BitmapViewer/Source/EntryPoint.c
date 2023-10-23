#include "Application.h"



Int32 EntryPoint()
{
	Runtime runtime;
	runtime.wantToQuit = False;

	EventSystem* eventSystem	= &runtime.eventSystem;
	*eventSystem				= FEventSystem.Construct(64);
	DynamicArray* eventQueue	= &eventSystem->eventQueue;

	Window* window	= &runtime.window;
	*window			= FWindow.Construct( L"BitmapViewer", 1280, 720, eventSystem );

	GraphicDeviceInterface gdi = FGraphicDeviceInterface.Construct( window->handle );

	OnAppInit( &runtime );

	// main loop
	{
		Timer		timer;
		RealType	timestep;
		{
			FTimer.Reset( &timer );

			timestep = 0;
		}

		while ( !runtime.wantToQuit )
		{
			timestep = FTimer.Elapsed( &timer );
			FTimer.Reset( &timer );

			FWindow.Update(window);
			for ( SizeType index = 0; index < eventQueue->count; index++ )
			{
				Event* event = CAST( Event*, FDynamicArray.IndexAt(eventQueue, index) );

				switch ( event->type )
				{
				case EventType_WindowResize:
				{
					window->width	= event->width;
					window->height	= event->height;

					OnAppResize( event->width, event->height );
				}
					break;
				case EventType_WindowFocus:
					OnAppFocus( event->isFocused );
					break;
				case EventType_WindowClose:
					OnAppQuit( event->wantToQuit );
					break;

				case EventType_Key:
					eventSystem->keys[ event->key ] |= event->keyState;
					break;

				case EventType_MouseButton:
					eventSystem->buttons[ event->button ] |= event->buttonState;
					break;
				case EventType_MouseMove:
					eventSystem->mousePosition[0] = event->mouseX;
					eventSystem->mousePosition[1] = event->mouseY;
					break;

				case EventType_MouseScrollWheelScroll:
					eventSystem->mouseScrollWheelScrolledValue += event->scrolledDelta;
					break;
				}
			}

			OnUpdate(timestep);
			FGraphicDeviceInterface.Present( &gdi, window->width, window->height, OnRender );

			FEventSystem.Reset(eventSystem);
		}
	}

	FGraphicDeviceInterface.Destruct( &gdi );
	FEventSystem.Destruct(eventSystem);

	return 0;
}


int main(int count, char** arguments)
{
	return EntryPoint();
}


#if defined(PLATFORM_WINDOWS)
#include <Windows.h>


int wWinMain(HINSTANCE current, HINSTANCE previous, PWSTR commandLine, int commandShow)
{
	return EntryPoint();
}
#endif