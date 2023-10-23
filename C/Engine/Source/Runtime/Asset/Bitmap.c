#include "Bitmap.h"



static const SizeType BitmapFileHeaderSize		= sizeof(BitmapFileHeader);
static const SizeType BitmapInfoHeaderSize		= sizeof(BitmapInfoHeader);
static const SizeType BitmapDefaultBitsOffset	= sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);


static Bitmap Construct(SizeType width, SizeType height, BitmapFormat format)
{
	SizeType channelCount = format == BitmapFormat_B8G8R8 ? 3 : 4;

	Bitmap ret;
	ret.buffer			= FAllocator.Allocate( channelCount * width * height );
	ret.width			= width;
	ret.height			= height;
	ret.format			= format;
	ret.channelCount	= channelCount;

	return ret;
}

static Void Destruct(Bitmap* instance)
{
	FAllocator.Release( instance->buffer );
}

static Void SetPixel(Bitmap* instance, SizeType x, SizeType y, UInt32 pixel)
{
	FMemory.Copy(
		&pixel,
		CAST( Byte*, instance->buffer ) + instance->channelCount * ( instance->width * y + x ),
		instance->channelCount
	);
}

static UInt32 GetPixel(const Bitmap* instance, SizeType x, SizeType y)
{
	UInt32 ret;
	FMemory.Copy(
		CAST( Byte*, instance->buffer ) + instance->channelCount * ( instance->width * y + x ),
		&ret,
		instance->channelCount
	);

	return ret;
}

static Void FlipHorizontal(Bitmap* instance)
{
	Byte* image				= CAST( Byte*, instance->buffer );
	SizeType width			= instance->width;
	SizeType height			= instance->height;
	SizeType channelCount	= instance->channelCount;

	SizeType halfWidth	= width / 2;
	for (SizeType y = 0; y < height; y++)
		for (SizeType x = 0; x < halfWidth; x++)
			for (SizeType index = 0; index < channelCount; index++)
				SWAP(
					Byte,
					image[ channelCount * ( width * y + x					) + index ],
					image[ channelCount * ( width * y + ( width - 1 - x )	) + index ]
				);
}

static Void FlipVertical(Bitmap* instance)
{
	Byte* image				= CAST( Byte*, instance->buffer );
	SizeType width			= instance->width;
	SizeType height			= instance->height;
	SizeType channelCount	= instance->channelCount;

	SizeType halfHeight	= height / 2;
	for (SizeType y = 0; y < halfHeight; y++)
		for (SizeType x = 0; x < width; x++)
			for (SizeType index = 0; index < instance->channelCount; index++)
				SWAP(
					Byte,
					image[ channelCount * ( width * y					+ x ) + index ],
					image[ channelCount * ( width * ( height - 1 - y )	+ x ) + index ]
				);
}

static Bool IsValid(const Void* buffer, SizeType byteCount)
{
	if ( byteCount < BitmapDefaultBitsOffset )
		return False;

	if ( !FMemory.Equal(buffer, "BM", 2) )
		return False;

	BitmapInfoHeader* infoHeader = REINTERPRET_CAST(const BitmapInfoHeader*, buffer, BitmapFileHeaderSize);
	if (
		infoHeader->bitCount != 24 &&
		infoHeader->bitCount != 32
		)
		return False;

	return True;
}

static Bitmap Decode(const Void* buffer, SizeType byteCount)
{
	BitmapInfoHeader* infoHeader = REINTERPRET_CAST(const BitmapInfoHeader*, buffer, BitmapFileHeaderSize);

	BitmapFormat format		= infoHeader->bitCount == 24 ? BitmapFormat_B8G8R8 : BitmapFormat_B8G8R8A8;
	SizeType channelCount	= infoHeader->bitCount / 8;

	Void* image = FAllocator.Allocate( infoHeader->imageSize );
	FMemory.Copy(
		CAST(const Byte*, buffer) + CAST(const BitmapFileHeader*, buffer)->offset,
		image,
		infoHeader->imageSize
	);

	Bitmap ret = { image, infoHeader->width, infoHeader->height, format, channelCount };
	return ret;
}

static HeapMemory Encode(const Bitmap* instance)
{
	SizeType imageSize = instance->channelCount * instance->width * instance->height;
	SizeType byteCount = BitmapDefaultBitsOffset + imageSize;

	HeapMemory ret = FHeapMemory.Construct(byteCount);
	{
		Void* buffer = ret.buffer;
		FMemory.Clear(buffer, BitmapDefaultBitsOffset);
		FMemory.Copy(
			instance->buffer,
			CAST(Byte*, buffer) + BitmapDefaultBitsOffset,
			imageSize
		);

		// set BitmapFileHeader
		BitmapFileHeader* fileHeader	= CAST(BitmapFileHeader*, buffer);
		fileHeader->magic[0]			= 'B';
		fileHeader->magic[1]			= 'M';
		fileHeader->size				= byteCount;
		fileHeader->offset				= BitmapDefaultBitsOffset;

		// set BitmapInfoHeader
		BitmapInfoHeader* infoHeader	= REINTERPRET_CAST(BitmapInfoHeader*, buffer, BitmapFileHeaderSize);
		infoHeader->size				= BitmapInfoHeaderSize;
		infoHeader->width				= instance->width;
		infoHeader->height				= instance->height;
		infoHeader->planes				= 1;
		infoHeader->bitCount			= 8 * instance->channelCount;
		infoHeader->imageSize			= imageSize;
	}

	return ret;
}


struct FBitmap FBitmap = 
{
	Construct,
	Destruct,
	SetPixel,
	GetPixel,
	FlipHorizontal,
	FlipVertical,
	IsValid,
	Decode,
	Encode,
};