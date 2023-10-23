#include "EventSystem.h"



namespace Engine
{
	EventSystem::EventSystem()
	{
		Memory::Clear( this, sizeof(EventSystem) );
	}

	EventSystem::~EventSystem()
	{

	}

	Bool EventSystem::KeyPressed(Key key) const
	{
		return ( keys[ static_cast<EnumUnderlyingType<Key>>(key) ] & KeyState::Press ) == KeyState::Press;
	}

	Bool EventSystem::KeyReleased(Key key) const
	{
		return ( keys[ static_cast<EnumUnderlyingType<Key>>(key) ] & KeyState::Release ) == KeyState::Release;
	}

	Bool EventSystem::KeyClicked(Key key) const
	{
		return ( keys[ static_cast<EnumUnderlyingType<Key>>(key) ] & KeyState::Click ) == KeyState::Click;
	}

	Bool EventSystem::MouseButtonPressed(MouseButton button) const
	{
		return ( buttons[ static_cast<EnumUnderlyingType<MouseButton>>(button) ] & MouseButtonState::Press ) == KeyState::Press;
	}

	Bool EventSystem::MouseButtonReleased(MouseButton button) const
	{
		return ( buttons[ static_cast<EnumUnderlyingType<MouseButton>>(button) ] & MouseButtonState::Release ) == KeyState::Release;
	}

	Bool EventSystem::MouseButtonClicked(MouseButton button) const
	{
		return ( buttons[ static_cast<EnumUnderlyingType<MouseButton>>(button) ] & MouseButtonState::Click ) == KeyState::Click;
	}

	Void EventSystem::AddWindowResizeEvent(SizeType width, SizeType height)
	{
		Event event;
		event.type		= Event::Type::WindowResize;
		event.width		= width;
		event.height	= height;

		eventQueue.Add(event);
	}

	Void EventSystem::AddWindowFocusEvent(Bool isFocused)
	{
		Event event;
		event.type		= Event::Type::WindowFocus;
		event.isFocused	= isFocused;

		eventQueue.Add(event);
	}

	Void EventSystem::AddWindowCloseEvent(Bool isClosed)
	{
		Event event;
		event.type		= Event::Type::WindowClose;
		event.isClosed	= isClosed;

		eventQueue.Add(event);
	}

	Void EventSystem::AddKeyEvent(Key key, KeyState state)
	{
		Event event;
		event.type		= Event::Type::Key;
		event.key		= key;
		event.keyState	= state;

		eventQueue.Add(event);
	}

	Void EventSystem::AddMouseButtonEvent(MouseButton button, MouseButtonState state)
	{
		Event event;
		event.type				= Event::Type::MouseButton;
		event.mouseButton		= button;
		event.mouseButtonState	= state;

		eventQueue.Add(event);
	}

	Void EventSystem::AddMouseMoveEvent(Int32 x, Int32 y)
	{
		Event event;
		event.type		= Event::Type::MouseMove;
		event.mouseX	= x;
		event.mouseY	= y;

		eventQueue.Add(event);
	}

	Void EventSystem::AddMouseScrollWheelScrollEvent(Int32 scrolledDelta)
	{
		Event event;
		event.type							= Event::Type::MouseScrollWheelScroll;
		event.mouseScrollWheelScrolledDelta	= scrolledDelta;

		eventQueue.Add(event);
	}

	Void EventSystem::Reset()
	{
		for ( SizeType index = 0; index < KeyCount; index++ )
			if ( ( keys[index] & KeyState::Click ) == KeyState::Click )
				keys[index] &= ~KeyState::Click;

		for ( SizeType index = 0; index < ButtonCount; index++ )
			if ( ( buttons[index] & MouseButtonState::Click ) == MouseButtonState::Click )
				buttons[index] &= ~MouseButtonState::Click;

		eventQueue.Clear();
	}
}