#include "Application.h"



static Runtime*		runtime;
static Window*		window;
static EventSystem*	eventSystem;


static Bool isLoaded = false;
static Bitmap bitmap;


Void OnAppInit(Void* userParameter)
{
	runtime		= reinterpret_cast<Runtime*>(userParameter);
	window		= &runtime->window;
	eventSystem	= &runtime->eventSystem;

	eventSystem->AddKeyEvent( Key::O, KeyState::Click );
}

Void OnAppResize(SizeType width, SizeType height)
{

}

Void OnAppFocused(Bool isFocused)
{
	runtime->isFocused = isFocused;
}

Void OnAppQuit(Bool wantToQuit)
{
	runtime->wantToQuit = wantToQuit;
}

Void OnUpdate(RealType deltaTime)
{
	if ( eventSystem->KeyClicked( Key::Escape ) )
		runtime->wantToQuit = true;

	if ( eventSystem->KeyClicked( Key::O ) )
	{
		isLoaded = true;

		U8String filename;
		{
			Char16 buffer[256];
			if ( !FileDialog::Open( buffer, sizeof(buffer) / sizeof(Char16), L"(*.bmp)\0*.bmp\0" ) )
			{
				isLoaded = false;
				return;
			}

			filename = U16String(buffer);
		}

		auto loaded = File::ReadAllBytes(filename);
		if ( !Bitmap::IsValid( loaded.GetBuffer(), loaded.GetByteCount() ) )
		{
			isLoaded = false;
			return;
		}
		
		bitmap = Bitmap::Decode( loaded.GetBuffer(), loaded.GetByteCount() );
	}
}

Void OnRender(Void* RenderTarget)
{
	if ( !isLoaded )
		return;

	SizeType width		= window->GetWidth();
	SizeType height		= window->GetHeight();
	SizeType centerX	= width / 2;
	SizeType centerY	= height / 2;

	RealType scale;
	{
		const RealType MaximumHorizontalScale	= static_cast<RealType>(width) / bitmap.GetWidth();
		const RealType MaximumVerticalScale		= static_cast<RealType>(height) / bitmap.GetHeight();

		scale = Minimum(MaximumHorizontalScale, MaximumVerticalScale);
	}

	SizeType imageWidth		= bitmap.GetWidth() * scale;
	SizeType imageHeight	= bitmap.GetHeight() * scale;

	SizeType xBegin	= centerX - imageWidth / 2;
	SizeType xEnd	= xBegin + imageWidth;
	SizeType yBegin	= centerY - imageHeight / 2;
	SizeType yEnd	= yBegin + imageHeight;

	for( SizeType y = yBegin; y < yEnd; y++ )
		for ( SizeType x = xBegin; x < xEnd; x++ )
		{
			RealType u = static_cast<RealType>( x - xBegin ) / ( imageWidth - 1 );
			RealType v = static_cast<RealType>( y - yBegin ) / ( imageHeight - 1 );

			reinterpret_cast<UInt32*>(RenderTarget)[ width * y + x ] =
				bitmap.GetPixel(
					( bitmap.GetWidth() - 1 ) * u,
					( bitmap.GetHeight() - 1 ) * v
				);
		}
}