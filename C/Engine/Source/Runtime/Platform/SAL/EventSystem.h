#pragma once

#include "../../Core.h"



// Reference from https://docs.microsoft.com/en-us/windows/win32/learnwin32/keyboard-input#key-codes
typedef enum
{
	Key_Unknown,

	// Control keys
	Key_BackSpace	= 0x08,
	Key_Tab			= 0x09,
	Key_Enter		= 0x0D,
	Key_Shift		= 0x10,
	Key_Ctrl		= 0x11,
	Key_CapsLock	= 0x14,
	Key_ESC			= 0x1B,
	Key_Space		= 0x20,

	// Arrow keys
	Key_LeftArrow = 0x25,
	Key_UpArrow,
	Key_RightArrow,
	Key_DownArrow,

	// 0 through 9 keys
	Key_Alpha0 = 0x30,
	Key_Alpha1,
	Key_Alpha2,
	Key_Alpha3,
	Key_Alpha4,
	Key_Alpha5,
	Key_Alpha6,
	Key_Alpha7,
	Key_Alpha8,
	Key_Alpha9,

	// A through Z keys
	Key_A = 0x41,
	Key_B,
	Key_C,
	Key_D,
	Key_E,
	Key_F,
	Key_G,
	Key_H,
	Key_I,
	Key_J,
	Key_K,
	Key_L,
	Key_M,
	Key_N,
	Key_O,
	Key_P,
	Key_Q,
	Key_R,
	Key_S,
	Key_T,
	Key_U,
	Key_V,
	Key_W,
	Key_X,
	Key_Y,
	Key_Z,

	// 0 through 9 keys(number pad)
	Key_NumberPad0 = 0x60,
	Key_NumberPad1,
	Key_NumberPad2,
	Key_NumberPad3,
	Key_NumberPad4,
	Key_NumberPad5,
	Key_NumberPad6,
	Key_NumberPad7,
	Key_NumberPad8,
	Key_NumberPad9,

	Key_Count,
}
Key;


typedef enum
{
	MouseButton_Unknown,

	MouseButton_Left	= 0x01,
	MouseButton_Right	= 0x02,
	MouseButton_Middle	= 0x10,

	MouseButton_Count,
}
MouseButton;


typedef enum
{
	KeyState_Press		= 1 << 0,
	KeyState_Release	= 1 << 1,
	KeyState_Click		= KeyState_Press | KeyState_Release,
}
KeyState;


typedef enum
{
	MouseButtonState_Press		= 1 << 0,
	MouseButtonState_Release	= 1 << 1,
	MouseButtonState_Click		= MouseButtonState_Press | MouseButtonState_Release,
}
MouseButtonState;


typedef enum
{
	EventType_Unknown,

	EventType_WindowResize,
	EventType_WindowFocus,
	EventType_WindowClose,

	EventType_Key,

	EventType_MouseButton,
	EventType_MouseMove,
	EventType_MouseScrollWheelScroll,
}
EventType;


typedef struct
{
	EventType type;


	union
	{
		// if type == EventType_WindowResized
		struct
		{
			SizeType width;
			SizeType height;
		};
		// if type == EventType_WindowClosing
		struct
		{
			Bool wantToQuit;
		};
		// if type == EventType_WindowFocused
		struct
		{
			Bool isFocused;
		};

		// if type == EventType_Key
		struct
		{
			Key			key;
			KeyState	keyState;
		};

		// if type == EventType_MouseButton
		struct
		{
			MouseButton		 button;
			MouseButtonState buttonState;
		};
		// if type == EventType_MouseMoved
		struct
		{
			Int32 mouseX;
			Int32 mouseY;
		};
		// if type == EventType_MouseScrollWheelScrolled
		struct
		{
			Int32 scrolledDelta;
		};
	};
}
Event;


typedef struct
{
	KeyState	keys[ Key_Count ];
	MouseButton	buttons[ MouseButton_Count ];

	Int32 mousePosition[2];
	Int32 mouseScrollWheelScrolledValue;

	DynamicArray eventQueue;
}
EventSystem;


struct FEventSystem
{
	EventSystem(*Construct)(SizeType eventQueueCapacity);
	Void(*Destruct)(EventSystem* instance);


	Bool(*KeyPressed)(const EventSystem* instance, Key key);
	Bool(*KeyReleased)(const EventSystem* instance, Key key);
	Bool(*KeyClicked)(const EventSystem* instance, Key key);

	Bool(*MouseButtonPressed)(const EventSystem* instance, MouseButton button);
	Bool(*MouseButtonReleased)(const EventSystem* instance, MouseButton button);
	Bool(*MouseButtonClicked)(const EventSystem* instance, MouseButton button);


	Void(*AddWindowResizeEvent)(EventSystem* instance, SizeType width, SizeType height);
	Void(*AddWindowFocusEvent)(EventSystem* instance, Bool isFocused);
	Void(*AddWindowCloseEvent)(EventSystem* instance, Bool wantToQuit);

	Void(*AddKeyEvent)(EventSystem* instance, Key key, KeyState state);

	Void(*AddMouseButtonEvent)(EventSystem* instance, MouseButton button, MouseButtonState state);
	Void(*AddMouseMoveEvent)(EventSystem* instance, Int32 x, Int32 y);
	Void(*AddMouseScrollWheelScrollEvent)(EventSystem* instance, Int32 scrolledDelta);

	Void(*Reset)(EventSystem* instance);
};


extern ENGINE_API struct FEventSystem FEventSystem;