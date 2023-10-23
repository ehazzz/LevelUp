#include "Application.h"



#pragma region Local
static Runtime*		runtime;
static Window*		window;
static EventSystem*	eventSystem;

static Serface RenderSerface;
#pragma endregion


Void OnAppInit(Void* userParam)
{
	runtime		= CAST(Runtime*, userParam);
	window		= &runtime->window;
	eventSystem	= &runtime->eventSystem;

	FMemory.Clear( &RenderSerface, sizeof(Serface) );

	GameInit(eventSystem);
}

Void OnAppResize(SizeType width, SizeType height)
{
	FSerface.Destruct( &RenderSerface );
	RenderSerface = FSerface.Construct( width, height, sizeof(Color) );
}

Void OnAppFocus(Bool isFocused)
{
	runtime->isFocused = isFocused;
}

Void OnAppQuit(Bool wantToQuit)
{
	runtime->wantToQuit = wantToQuit;
}

Void OnUpdate(RealType deltaTime)
{
	GameUpdate(deltaTime);
}

Void OnRender(Void* RenderTarget)
{
	// frame begin
	Color clearColor = FColor.Multiply( &FColor.White, 0.5 );
	FSerface.Clear( &RenderSerface, &clearColor );

	// do your rendering work here(by updating RenderSerface)
	{
		GameRender( &RenderSerface );
	}

	// frame end
	{
		typedef struct { Byte b, g, r, a; } BGRA;


		for ( SizeType index = 0; index < RenderSerface.elementCount; index++ )
		{
			Color* from	= REINTERPRET_CAST( Color*, RenderSerface.buffer, RenderSerface.datatypeSize * index );
			BGRA* to	= REINTERPRET_CAST( BGRA*, RenderTarget, sizeof(BGRA) * index );

			to->b = FMath.Clamp01( from->b ) * 255;
			to->g = FMath.Clamp01( from->g ) * 255;
			to->r = FMath.Clamp01( from->r ) * 255;
			to->a = FMath.Clamp01( from->a ) * 255;
		}
	}
}
