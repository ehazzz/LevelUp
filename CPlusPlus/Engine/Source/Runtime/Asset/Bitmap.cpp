#include "Bitmap.h"



namespace Engine
{
	Bitmap::Bitmap()
	{
		Memory::Clear( this, sizeof(Bitmap) );
	}

	Bitmap::Bitmap(SizeType width, SizeType height, Format format)
	{
		SizeType channelCount	= format == Format::B8G8R8 ? 3 : 4;
		SizeType imageSize		= channelCount * width * height;

		this->buffer		= Allocator::Allocate<Byte>(imageSize);
		this->width			= width;
		this->height		= height;
		this->format		= format;
		this->channelCount	= channelCount;
	}

	Bitmap::Bitmap(const Bitmap& other)
	{
		CopyFrom(other);
	}

	Bitmap::Bitmap(Bitmap&& other)
	{
		buffer			= other.buffer;
		width			= other.width;
		height			= other.height;
		format			= other.format;
		channelCount	= other.channelCount;

		other.buffer = nullptr;
	}

	Bitmap::~Bitmap()
	{
		Allocator::Release( reinterpret_cast<Byte*>(buffer) );
	}

	Void Bitmap::SetPixel(SizeType x, SizeType y, UInt32 pixel)
	{
		Memory::Copy(
			&pixel,
			reinterpret_cast<Byte*>(buffer) + channelCount * ( width * y + x ),
			channelCount
		);
	}

	UInt32 Bitmap::GetPixel(SizeType x, SizeType y) const
	{
		UInt32 ret;
		Memory::Copy(
			reinterpret_cast<Byte*>(buffer) + channelCount * ( width * y + x ),
			&ret,
			channelCount
		);

		return ret;
	}

	Void Bitmap::FlipHorizontal()
	{
		SizeType halfWidth = width / 2;
		for ( SizeType y = 0; y < height; y++ )
			for ( SizeType x = 0; x < halfWidth; x++ )
				for ( SizeType index = 0; index < channelCount; index++ )
					Swap(
						reinterpret_cast<Byte*>(buffer) + channelCount * ( width * y + x					) + index,
						reinterpret_cast<Byte*>(buffer) + channelCount * ( width * y + ( width - 1 - x )	) + index
					);
	}

	Void Bitmap::FlipVertical()
	{
		SizeType halfHeight = height / 2;
		for ( SizeType y = 0; y < halfHeight; y++ )
			for ( SizeType x = 0; x < width; x++ )
				for ( SizeType index = 0; index < channelCount; index++ )
					Swap(
						reinterpret_cast<Byte*>(buffer) + channelCount * ( width * y					+ x ) + index,
						reinterpret_cast<Byte*>(buffer) + channelCount * ( width * ( height - 1 - y )	+ x ) + index
					);
	}

	Bool Bitmap::IsValid(const Void* buffer, SizeType byteCount)
	{
		if ( byteCount < DefaultBitsOffset )
			return false;

		if ( !Memory::Equal(buffer, "BM", 2) )
			return false;

		auto infoHeader = reinterpret_cast<const InfoHeader*>( reinterpret_cast<const Byte*>(buffer) + FileHeaderSize );
		if (
			infoHeader->bitCount != 24 &&
			infoHeader->bitCount != 32
			)
			return false;

		return true;
	}

	Bitmap Bitmap::Decode(const Void* buffer, SizeType byteCount)
	{
		auto infoHeader	= reinterpret_cast<const InfoHeader*>( reinterpret_cast<const Byte*>(buffer) + FileHeaderSize );

		SizeType width			= infoHeader->width;
		SizeType height			= infoHeader->height;
		Format format			= infoHeader->bitCount == 24 ? Format::B8G8R8 : Format::B8G8R8A8;
		SizeType channelCount	= infoHeader->bitCount / 8;

		SizeType imageSize = channelCount * width * height;

		Bitmap ret(width, height, format);
		Memory::Copy(
			reinterpret_cast<const Byte*>(buffer) + reinterpret_cast<const FileHeader*>(buffer)->offset,
			ret.buffer,
			imageSize
		);
		
		return ret;
	}

	HeapMemory Bitmap::Encode(const Bitmap& instance)
	{
		SizeType width			= instance.width;
		SizeType height			= instance.height;
		SizeType channelCount	= instance.channelCount;

		SizeType imageSize = channelCount * width * height;
		SizeType totalSize = DefaultBitsOffset + imageSize;

		HeapMemory ret(totalSize);
		{
			auto buffer = ret.GetBuffer();
			Memory::Clear(
				buffer,
				DefaultBitsOffset
			);
			Memory::Copy(
				instance.GetBuffer(),
				reinterpret_cast<Byte*>(buffer) + DefaultBitsOffset,
				imageSize
			);

			// set FileHeader
			{
				auto descriptor			= reinterpret_cast<FileHeader*>(buffer);
				descriptor->magic[0]	= 'B';
				descriptor->magic[1]	= 'M';
				descriptor->size		= totalSize;
				descriptor->offset		= DefaultBitsOffset;
			}

			// set InfoHeader
			{
				auto descriptor			= reinterpret_cast<InfoHeader*>( reinterpret_cast<Byte*>(buffer) + FileHeaderSize );
				descriptor->size		= InfoHeaderSize;
				descriptor->width		= width;
				descriptor->height		= height;
				descriptor->planes		= 1;
				descriptor->bitCount	= 8 * channelCount;
				descriptor->imageSize	= imageSize;
			}
		}

		return ret;
	}

	Void* Bitmap::GetBuffer()
	{
		return buffer;
	}

	const Void* Bitmap::GetBuffer() const
	{
		return buffer;
	}

	SizeType Bitmap::GetWidth() const
	{
		return width;
	}

	SizeType Bitmap::GetHeight() const
	{
		return height;
	}

	Bitmap::Format Bitmap::GetFormat() const
	{
		return format;
	}

	SizeType Bitmap::GetChannelCount() const
	{
		return channelCount;
	}

	Bitmap& Bitmap::operator=(const Bitmap& other)
	{
		this->~Bitmap();
		CopyFrom(other);

		return *this;
	}

	Bitmap& Bitmap::operator=(Bitmap&& other)
	{
		Swap( &buffer,			&other.buffer		);
		Swap( &width,			&other.width		);
		Swap( &height,			&other.height		);
		Swap( &format,			&other.format		);
		Swap( &channelCount,	&other.channelCount	);

		return *this;
	}

	Void Bitmap::CopyFrom(const Bitmap& instance)
	{
		SizeType imageSize = instance.channelCount * instance.width * instance.height;

		buffer			= Allocator::Allocate<Byte>(imageSize);
		width			= instance.width;
		height			= instance.height;
		format			= instance.format;
		channelCount	= instance.channelCount;

		Memory::Copy(
			instance.buffer,
			buffer,
			imageSize
		);
	}
}