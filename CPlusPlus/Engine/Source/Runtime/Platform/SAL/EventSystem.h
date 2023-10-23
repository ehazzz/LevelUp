#pragma once

#include "../../Core.h"



namespace Engine
{
	// Reference from https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	enum class Key
	{
		// Control keys
		Backspace	= 0x08,
		Tab			= 0x09,
		Shift		= 0x10,
		Control		= 0x11, // CTRL key
		Menu		= 0x12, // ALT key
		Capital		= 0x14, // Caps lock key
		Escape		= 0x1B, // Esc key
		Space		= 0x20,

		// Arrow keys
		LeftArrow = 0x25,
		UpArrow,
		RightArrow,
		DownArrow,
		
		// 0 - 9 keys
		Alpha0 = 0x30,
		Alpha1,
		Alpha2,
		Alpha3,
		Alpha4,
		Alpha5,
		Alpha6,
		Alpha7,
		Alpha8,
		Alpha9,

		// A - Z keys
		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		// 0 - 9 keys(number pad)
		NumberPad0 = 0x60,
		NumberPad1,
		NumberPad2,
		NumberPad3,
		NumberPad4,
		NumberPad5,
		NumberPad6,
		NumberPad7,
		NumberPad8,
		NumberPad9,

		// Function keys
		Fn1 = 0x70,
		Fn2,
		Fn3,
		Fn4,
		Fn5,
		Fn6,
		Fn7,
		Fn8,
		Fn9,
		Fn10,
		Fn11,
		Fn12,

		Count,
	};


	enum class MouseButton
	{
		Left	= 0x01,
		Right	= 0x02,
		Middle	= 0x10,

		Count,
	};


	enum class KeyState
	{
		Press	= 1 << 0,
		Release	= 1 << 1,

		Click	= Press | Release,
	};
	ENABLE_BITWISE_OPERATORS(KeyState);


	using MouseButtonState = KeyState;


	struct Event
	{
		enum class Type
		{
			WindowResize,
			WindowFocus,
			WindowClose,

			Key,

			MouseButton,
			MouseMove,
			MouseScrollWheelScroll,
		};


		Type type;


		union
		{
			// if type == Type::WindowResize
			struct
			{
				SizeType width;
				SizeType height;
			};

			// if type == Type::WindowFocus
			struct
			{
				Bool isFocused;
			};

			// if type == Type::WindowClose
			struct
			{
				Bool isClosed;
			};

			// if type == Type::Key
			struct
			{
				Key			key;
				KeyState	keyState;
			};

			// if type == Type::MouseButton
			struct
			{
				MouseButton			mouseButton;
				MouseButtonState	mouseButtonState;
			};

			// if type == Type::MouseMove
			struct
			{
				Int32 mouseX;
				Int32 mouseY;
			};

			// if type == Type::MouseButton
			struct
			{
				Int32 mouseScrollWheelScrolledDelta;
			};
		};
	};


	class ENGINE_API EventSystem
	{
	public:
		NONCOPYABLE(EventSystem);


		EventSystem();
		~EventSystem();


		Bool KeyPressed(Key key) const;
		Bool KeyReleased(Key key) const;
		Bool KeyClicked(Key key) const;

		Bool MouseButtonPressed(MouseButton button) const;
		Bool MouseButtonReleased(MouseButton button) const;
		Bool MouseButtonClicked(MouseButton button) const;


		Void AddWindowResizeEvent(SizeType width, SizeType height);
		Void AddWindowFocusEvent(Bool isFocused);
		Void AddWindowCloseEvent(Bool isClosed);

		Void AddKeyEvent(Key key, KeyState state);
		
		Void AddMouseButtonEvent(MouseButton button, MouseButtonState state);
		Void AddMouseMoveEvent(Int32 x, Int32 y);
		Void AddMouseScrollWheelScrollEvent(Int32 scrolledDelta);

		Void Reset();
	public:
		static constexpr auto KeyCount		= static_cast<EnumUnderlyingType<Key>>( Key::Count );
		static constexpr auto ButtonCount	= static_cast<EnumUnderlyingType<MouseButton>>( MouseButton::Count );


		KeyState			keys[KeyCount];
		MouseButtonState	buttons[ButtonCount];

		Int32 mousePosition[2];
		Int32 mouseScrollWheelScrolledValue;

		DynamicArray<Event> eventQueue;
	};
}