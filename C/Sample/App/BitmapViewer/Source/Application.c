#include "Application.h"



static Runtime*		runtime;
static Window*		window;
static EventSystem*	eventSystem;


static Bool isLoaded;
static Bitmap bitmap;


Void OnAppInit(Void* userParam)
{
	runtime		= CAST(Runtime*, userParam);
	window		= &runtime->window;
	eventSystem	= &runtime->eventSystem;

	{
		isLoaded = False;
		FMemory.Clear( &bitmap, sizeof(Bitmap) );

		FEventSystem.AddKeyEvent(eventSystem, Key_O, KeyState_Click);
	}
}

Void OnAppResize(SizeType width, SizeType height)
{

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
	if( FEventSystem.KeyClicked( eventSystem, Key_ESC ) )
		runtime->wantToQuit = True;

	if ( FEventSystem.KeyClicked( eventSystem, Key_O ) )
	{
		Char16 filename[512];
		if ( FFileDialog.Open( filename, sizeof(filename) / sizeof(Char16), L"(*.bmp)\0*.bmp\0", L"./", L"Open *.bmp", window->handle ) )
		{
			FBitmap.Destruct( &bitmap );

			U16String u16Filename	= FU16String.Construct(filename);
			U8String path			= FU16String.ToU8String( &u16Filename );

			HeapMemory loaded = FFile.ReadAllBytes( &path );
			if ( FBitmap.IsValid( loaded.buffer, loaded.byteCount ) )
			{
				isLoaded = True;
				bitmap = FBitmap.Decode( loaded.buffer, loaded.byteCount );

			}
			else
				isLoaded = False;

			FHeapMemory.Destruct( &loaded );

			FU8String.Destruct( &path );
			FU16String.Destruct( &u16Filename );
		}
	}
}

Void OnRender(Void* RenderTarget)
{
	if ( !isLoaded )
		return;

	SizeType width		= window->width;
	SizeType height		= window->height;
	SizeType centerX	= width / 2;
	SizeType centerY	= height / 2;

	RealType scale;
	{
		const RealType MaximumHorizontalScale	= CAST(RealType, width) / bitmap.width;
		const RealType MaximumVerticalScale		= CAST(RealType, height) / bitmap.height;

		scale = FMath.Minimum(MaximumHorizontalScale, MaximumVerticalScale);
	}

	SizeType imageWidth		= bitmap.width * scale;
	SizeType imageHeight	= bitmap.height * scale;

	Int32 xBegin	= centerX - imageWidth / 2;
	Int32 xEnd		= xBegin + imageWidth;
	Int32 yBegin	= centerY - imageHeight / 2;
	Int32 yEnd		= yBegin + imageHeight;

	for ( Int32 y = yBegin; y < yEnd; y++ )
		for ( Int32 x = xBegin; x < xEnd; x++ )
		{
			RealType u = CAST( RealType, x - xBegin ) / ( imageWidth - 1 );
			RealType v = CAST( RealType, y - yBegin ) / ( imageHeight - 1 );

			UInt32 pixel = FBitmap.GetPixel( &bitmap, ( bitmap.width - 1 ) * u, ( bitmap.height - 1 ) * v );

			CAST(UInt32*, RenderTarget)[ width * y + x ] = pixel;
		}
}
