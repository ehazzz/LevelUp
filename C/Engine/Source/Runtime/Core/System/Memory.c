#include "Memory.h"



typedef UInt64 AlignType;


static const SizeType AlignSize = sizeof(AlignType);


static Void Clear(Void* buffer, SizeType byteCount)
{
	SizeType count	= byteCount / AlignSize;
	SizeType mod	= byteCount % AlignSize;
	SizeType offset = byteCount - mod;

	for (SizeType index = 0; index < count; index++)
		CAST(AlignType*, buffer)[index] = 0;

	for (SizeType index = 0; index < mod; index++)
		CAST(Byte*, buffer)[ offset + index ] = 0;
}

static Void Copy(const Void* from, Void* to, SizeType byteCount)
{
	SizeType count	= byteCount / AlignSize;
	SizeType mod	= byteCount % AlignSize;
	SizeType offset = byteCount - mod;

	for (SizeType index = 0; index < count; index++)
		CAST(AlignType*, to)[index] = CAST(const AlignType*, from)[index];

	for (SizeType index = 0; index < mod; index++)
		CAST(Byte*, to)[ offset + index ] = CAST(const Byte*, from)[ offset + index ];
}

static Bool Equal(const Void* bufferA, const Void* bufferB, SizeType byteCount)
{
	SizeType count	= byteCount / AlignSize;
	SizeType mod	= byteCount % AlignSize;
	SizeType offset = byteCount - mod;

	for (SizeType index = 0; index < count; index++)
		if (
			CAST(const AlignType*, bufferA)[index] !=
			CAST(const AlignType*, bufferB)[index]
			)
			return False;

	for (SizeType index = 0; index < mod; index++)
		if (
			CAST(const Byte*, bufferA)[ offset + index ] !=
			CAST(const Byte*, bufferB)[ offset + index ]
			)
			return False;

	return True;
}


struct FMemory FMemory =
{
	Clear,
	Copy,
	Equal,
};