#include "Memory.h"



namespace Engine
{
	using AlignType = UInt64;


	constexpr SizeType AlignSize = sizeof(AlignType);


	Void Memory::Clear(Void* buffer, SizeType byteCount)
	{
		SizeType count	= byteCount / AlignSize;
		SizeType mod	= byteCount % AlignSize;
		SizeType offset	= byteCount - mod;

		for ( SizeType index = 0; index < count; index++ )
			reinterpret_cast<AlignType*>(buffer)[index] = AlignType();

		for ( SizeType index = 0; index < mod; index++ )
			reinterpret_cast<Byte*>(buffer)[ offset + index ] = Byte();
	}

	Void Memory::Copy(const Void* from, Void* to, SizeType byteCount)
	{
		SizeType count	= byteCount / AlignSize;
		SizeType mod	= byteCount % AlignSize;
		SizeType offset	= byteCount - mod;

		for ( SizeType index = 0; index < count; index++ )
			reinterpret_cast<AlignType*>(to)[index] = reinterpret_cast<const AlignType*>(from)[index];

		for ( SizeType index = 0; index < mod; index++ )
			reinterpret_cast<Byte*>(to)[ offset + index ] = reinterpret_cast<const Byte*>(from)[ offset + index ];
	}

	Bool Memory::Equal(const Void* bufferA, const Void* bufferB, SizeType byteCount)
	{
		SizeType count	= byteCount / AlignSize;
		SizeType mod	= byteCount % AlignSize;
		SizeType offset	= byteCount - mod;

		for ( SizeType index = 0; index < count; index++ )
			if (
				reinterpret_cast<const AlignType*>(bufferA)[index] !=
				reinterpret_cast<const AlignType*>(bufferB)[index]
				)
				return false;

		for ( SizeType index = 0; index < mod; index++ )
			if (
				reinterpret_cast<const Byte*>(bufferA)[ offset + index ] !=
				reinterpret_cast<const Byte*>(bufferB)[ offset + index ]
				)
				return false;

		return true;
	}
}