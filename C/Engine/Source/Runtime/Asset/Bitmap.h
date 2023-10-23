#pragma once

/**
* Used To Decode/Encode Uncompressed 24-bit(B8G8R8) or 32-bit(B8G8R8A8) *.bmp File
*/

#include "../Core.h"



/**
* Reference from https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
* 
* 
* @remark
* The size in bytes of the struct must be 14
*/
#pragma pack(push, 2)
typedef struct
{
	Byte	magic[2];	// Must be "BM"
	UInt32	size;		// Indicate the size in bytes of *.bmp file
	UInt32	reserved;	// Unused
	UInt32	offset;		// The offset to image pixels, default is 54(14 + 40)
}
BitmapFileHeader;
#pragma pack(pop)


/**
* Reference from https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
* 
* 
* @remark
* The size in bytes of the struct should be 40
*/
typedef struct
{
	UInt32 size;
	UInt32 width;
	UInt32 height;
	UInt16 planes;
	UInt16 bitCount;
	UInt32 compression;
	UInt32 imageSize;
	UInt32 xPixelsPerMeter;
	UInt32 yPixelsPermeter;
	UInt32 colorUsed;
	UInt32 colorImportant;
}
BitmapInfoHeader;


typedef enum
{
	BitmapFormat_B8G8R8,
	BitmapFormat_B8G8R8A8,
}
BitmapFormat;


typedef struct
{
	Void* buffer;

	SizeType		width;
	SizeType		height;
	BitmapFormat	format;
	SizeType		channelCount;
}
Bitmap;


struct FBitmap
{
	Bitmap(*Construct)(SizeType width, SizeType height, BitmapFormat format);
	Void(*Destruct)(Bitmap* instance);


	/**
	* @remark
	* Pixel's high 8-bit is unused if instance->format == BitmapFormat_B8G8R8
	* 
	* 
	* typedef struct { Byte b, g, r;	} BitmapPixel24;
	* typedef struct { Byte b, g, r, a; } BitmapPixel32;
	* 
	* Byte b, g, r, a
	* UInt32 pixel = ( b << 0 ) | ( g << 8 ) | ( r << 16 ) | ( a << 24 )
	*/
	Void(*SetPixel)(Bitmap* instance, SizeType x, SizeType y, UInt32 pixel);
	UInt32(*GetPixel)(const Bitmap* instance, SizeType x, SizeType y);

	Void(*FlipHorizontal)(Bitmap* instance);
	Void(*FlipVertical)(Bitmap* instance);


	// Test if a pointer point to heap memory is a valid *.bmp file representation
	Bool(*IsValid)(const Void* buffer, SizeType byteCount);

	Bitmap(*Decode)(const Void* buffer, SizeType byteCount);
	HeapMemory(*Encode)(const Bitmap* instance);
};


extern ENGINE_API struct FBitmap FBitmap;