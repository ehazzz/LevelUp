#include "U16String.h"

#include "U8String.h"
#include "U32String.h"



static U16String Construct(const Char16* utf16)
{
	SizeType count = FUnicode.GetUTF16ElementCount(utf16, 0x0000);
	Char16* buffer = CAST( Char16*, FAllocator.Allocate( sizeof(Char16) * ( count + 1 ) ) );

	FMemory.Copy( utf16, buffer, sizeof(Char16) * ( count + 1 ) );

	U16String ret = { buffer, count };
	return ret;
}

static Void Destruct(U16String* instance)
{
	FAllocator.Release( instance->buffer );
}

static U8String ToU8String(const U16String* instance)
{
	U32String u32	= FU16String.ToU32String(instance);
	U8String u8		= FU32String.ToU8String( &u32 );

	FU32String.Destruct( &u32 );

	return u8;
}

static U32String ToU32String(const U16String* instance)
{
	SizeType count = FUnicode.GetCodePointCountFromUTF16Elements( instance->buffer, instance->count );
	Char32* buffer = CAST( Char32*, FAllocator.Allocate( sizeof(Char32) * ( count + 1 ) ) );

	// insert terminated character
	buffer[count] = '\0';

	SizeType offset = 0;
	for (SizeType index = 0; index < count; index++)
		offset += FUnicode.FromUTF16(
			buffer + index,
			instance->buffer + offset
		);

	U32String ret = { buffer, count };
	return ret;
}


struct FU16String FU16String = 
{
	Construct,
	Destruct,
	ToU8String,
	ToU32String,
};
