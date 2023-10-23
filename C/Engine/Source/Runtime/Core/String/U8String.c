#include "U8String.h"

#include "U16String.h"
#include "U32String.h"



static U8String Construct(const Char8* utf8)
{
	SizeType count	= FUnicode.GetUTF8ElementCount(utf8, 0x00);
	Char8* buffer	= CAST( Char8*, FAllocator.Allocate( sizeof(Char8) * ( count + 1 ) ) );

	FMemory.Copy( utf8, buffer, sizeof(Char8) * ( count + 1 ) );

	U8String ret = { buffer, count };
	return ret;
}

static Void Destruct(U8String* instance)
{
	if ( !instance )
		return;

	FAllocator.Release( instance->buffer );
}

static U16String ToU16String(const U8String* instance)
{
	U32String u32 = FU8String.ToU32String(instance);
	U16String u16 = FU32String.ToU16String( &u32 );

	FU32String.Destruct( &u32 );

	return u16;
}

static U32String ToU32String(const U8String* instance)
{
	SizeType count = FUnicode.GetCodePointCountFromUTF8Elements( instance->buffer, instance->count );
	Char32* buffer = CAST( Char32*, FAllocator.Allocate( sizeof(Char32) * ( count + 1 ) ) );

	buffer[count] = '\0';

	SizeType offset = 0;
	for (SizeType index = 0; index < count; index++)
		offset += FUnicode.FromUTF8(
			buffer + index,
			instance->buffer + offset
		);

	U32String ret = { buffer, count };
	return ret;
}


struct FU8String FU8String = 
{
	Construct,
	Destruct,
	ToU16String,
	ToU32String,
};
