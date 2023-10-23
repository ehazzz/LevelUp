#include "Icon.h"

#include "Windows.h"



namespace Engine::Windows
{
	Icon::Icon()
		: handle(nullptr)
	{

	}

	Icon::Icon(const Bitmap& bitmap, Bool asSystemCursor, SizeType xHotspot, SizeType yHotspot)
	{
		SizeType width			= bitmap.GetWidth();
		SizeType height			= bitmap.GetHeight();
		SizeType channelCount	= bitmap.GetChannelCount();
		SizeType imageSize		= channelCount * width * height;


		HDC dc = GetDC(nullptr);

		HBITMAP mask;
		HBITMAP color;
		{
			if ( channelCount == 4 )
			{
				SizeType size	= 1 * width * height;
				auto buffer		= Allocator::Allocate<Byte>(size);

				for ( SizeType y = 0; y < height; y++ )
					for ( SizeType x = 0; x < height; x++ )
						buffer[ width * y + x ] = bitmap.GetPixel(x, y) >> 24;

				mask = CreateBitmap(width, height, 1, 8, buffer);
				Allocator::Release(buffer);
			}
			else
				mask = CreateBitmap(width, height, 1, 1, nullptr);

			{
				BITMAPINFO descriptor;
				Memory::Clear( &descriptor, sizeof(BITMAPINFO) );
				descriptor.bmiHeader.biSize		= sizeof(BITMAPINFO);
				descriptor.bmiHeader.biWidth	= width;
				descriptor.bmiHeader.biHeight	= height;
				descriptor.bmiHeader.biBitCount	= 8 * channelCount;
				descriptor.bmiHeader.biPlanes	= 1;

				Void* target;
				color = CreateDIBSection( dc, &descriptor, DIB_RGB_COLORS, &target, nullptr, 0 );
				
				Memory::Copy(
					bitmap.GetBuffer(),
					target,
					imageSize
				);
			}
		}

		{
			ICONINFO descriptor;
			Memory::Clear( &descriptor, sizeof(ICONINFO) );
			descriptor.fIcon	= !asSystemCursor;
			descriptor.xHotspot = xHotspot;
			descriptor.yHotspot = height - 1 - yHotspot;
			descriptor.hbmMask	= mask;
			descriptor.hbmColor	= color;

			handle = CreateIconIndirect( &descriptor );
		}

		DeleteObject(mask);
		DeleteObject(color);
		ReleaseDC(nullptr, dc);
	}

	Icon::Icon(const Char16* executableFilePath)
	{
		handle = ExtractIconW(nullptr, executableFilePath, 0);
	}

	Icon::~Icon()
	{
		DestroyIcon( reinterpret_cast<HICON>(handle) );
	}

	Void Icon::AttachToWindow(Void* nativeWindowsWindowHandle)
	{
		SendMessageW(
			reinterpret_cast<HWND>(nativeWindowsWindowHandle),
			WM_SETICON,
			ICON_BIG,
			reinterpret_cast<LPARAM>(handle)
		);
	}

	Void Icon::SetAsSystemCursor()
	{
		SetCursor( reinterpret_cast<HCURSOR>(handle) );
	}

	Bitmap Icon::ExtractToBitmap()
	{
		// extract image pixels from HBITMAP
		auto Fn = [](HBITMAP hBitmap, HDC dc)
			{
				SizeType width;
				SizeType height;
				SizeType channelCount;
				{
					BITMAP descriptor;
					GetObjectW( hBitmap, sizeof(BITMAP), &descriptor );

					width			= descriptor.bmWidth;
					height			= descriptor.bmHeight;
					channelCount	= descriptor.bmBitsPixel / 8;
				}

				Bitmap ret(
					width,
					height,
					channelCount == 3 ? Bitmap::Format::B8G8R8 : Bitmap::Format::B8G8R8A8
				);
				{
					BITMAPINFO descriptor;
					Memory::Clear( &descriptor, sizeof(BITMAPINFO) );
					descriptor.bmiHeader.biSize		= sizeof(BITMAPINFO);
					descriptor.bmiHeader.biWidth	= width;
					descriptor.bmiHeader.biHeight	= height;
					descriptor.bmiHeader.biPlanes	= 1;
					descriptor.bmiHeader.biBitCount	= 8 * channelCount;

					GetDIBits( dc, hBitmap, 0, height, ret.GetBuffer(), &descriptor, DIB_RGB_COLORS );
				}

				return ret;
			};


		HDC dc = GetDC(nullptr);

		HBITMAP mask;
		HBITMAP color;
		{
			ICONINFO descriptor;
			GetIconInfo( reinterpret_cast<HICON>(handle), &descriptor );

			mask	= descriptor.hbmMask;
			color	= descriptor.hbmColor;
		}

		auto ret = Fn(color, dc);

		DeleteObject(mask);
		DeleteObject(color);
		ReleaseDC(nullptr, dc);

		return ret;
	}

	Void* Icon::GetHandle() const
	{
		return handle;
	}
}