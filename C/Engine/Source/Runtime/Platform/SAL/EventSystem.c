#include "EventSystem.h"



static EventSystem Construct(SizeType eventQueueCapacity)
{
	EventSystem ret;
	FMemory.Clear( &ret, sizeof(EventSystem) );
	ret.eventQueue = FDynamicArray.Construct( eventQueueCapacity, sizeof(Event) );

	return ret;
}

static Void Destruct(EventSystem* instance)
{
	if ( !instance )
		return;

	FDynamicArray.Destruct( &instance->eventQueue );
}

static Bool KeyPressed(const EventSystem* instance, Key key)
{
	return ( instance->keys[key] & KeyState_Press ) == KeyState_Press;
}

static Bool KeyReleased(const EventSystem* instance, Key key)
{
	return ( instance->keys[key] & KeyState_Release ) == KeyState_Release;
}

static Bool KeyClicked(const EventSystem* instance, Key key)
{
	return ( instance->keys[key] & KeyState_Click ) == KeyState_Click;
}

static Bool MouseButtonPressed(const EventSystem* instance, MouseButton button)
{
	return ( instance->buttons[button] & MouseButtonState_Press ) == MouseButtonState_Press;
}

static Bool MouseButtonReleased(const EventSystem* instance, MouseButton button)
{
	return ( instance->buttons[button] & MouseButtonState_Release ) == MouseButtonState_Release;
}

static Bool MouseButtonClicked(const EventSystem* instance, MouseButton button)
{
	return ( instance->buttons[button] & MouseButtonState_Click ) == MouseButtonState_Click;
}

static Void AddWindowResizeEvent(EventSystem* instance, SizeType width, SizeType height)
{
	Event event;
	event.type		= EventType_WindowResize;
	event.width		= width;
	event.height	= height;

	FDynamicArray.Add( &instance->eventQueue, &event );
}


static Void AddWindowFocusEvent(EventSystem* instance, Bool isFocused)
{
	Event event;
	event.type		= EventType_WindowFocus;
	event.isFocused = isFocused;

	FDynamicArray.Add( &instance->eventQueue, &event );
}

static Void AddWindowCloseEvent(EventSystem* instance, Bool wantToQuit)
{
	Event event;
	event.type			= EventType_WindowClose;
	event.wantToQuit	= wantToQuit;

	FDynamicArray.Add( &instance->eventQueue, &event );
}

static Void AddKeyEvent(EventSystem* instance, Key key, KeyState state)
{
	Event event;
	event.type		= EventType_Key;
	event.key		= key;
	event.keyState	= state;

	FDynamicArray.Add( &instance->eventQueue, &event );
}

static Void AddMouseButtonEvent(EventSystem* instance, MouseButton button, MouseButtonState state)
{
	Event event;
	event.type			= EventType_MouseButton;
	event.button		= button;
	event.buttonState	= state;

	FDynamicArray.Add( &instance->eventQueue, &event );
}

static Void AddMouseMoveEvent(EventSystem* instance, Int32 x, Int32 y)
{
	Event event;
	event.type		= EventType_MouseMove;
	event.mouseX	= x;
	event.mouseY	= y;

	FDynamicArray.Add( &instance->eventQueue, &event );
}

static Void AddMouseScrollWheelScrollEvent(EventSystem* instance, Int32 scrolledDelta)
{
	Event event;
	event.type			= EventType_MouseScrollWheelScroll;
	event.scrolledDelta	= scrolledDelta;

	FDynamicArray.Add( &instance->eventQueue, &event );
}

static Void Reset(EventSystem* instance)
{
	for (SizeType index = 0; index < Key_Count; index++)
		if ( ( instance->keys[index] & KeyState_Click ) == KeyState_Click )
			instance->keys[index] &= ~KeyState_Click;

	for (SizeType index = 0; index < MouseButton_Count; index++)
		if ( ( instance->buttons[index] & MouseButtonState_Click ) == MouseButtonState_Click )
			instance->buttons[index] &= ~MouseButtonState_Click;

	FDynamicArray.Clear( &instance->eventQueue );
}


struct FEventSystem FEventSystem = 
{
	Construct,
	Destruct,
	KeyPressed,
	KeyReleased,
	KeyClicked,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseButtonClicked,
	AddWindowResizeEvent,
	AddWindowFocusEvent,
	AddWindowCloseEvent,
	AddKeyEvent,
	AddMouseButtonEvent,
	AddMouseMoveEvent,
	AddMouseScrollWheelScrollEvent,
	Reset,
};
