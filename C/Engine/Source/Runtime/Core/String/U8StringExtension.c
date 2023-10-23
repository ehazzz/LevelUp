#include "U8StringExtension.h"



static Int32 IndexOf(const U8String* instance, Char8 character)
{
	for (SizeType index = 0; index < instance->count; index++)
		if ( instance->buffer[index] == character )
			return index;

	return -1;
}

static Int32 LastIndexOf(const U8String* instance, Char8 character)
{
	for(Int32 index = instance->count - 1; index >= 0; index--)
		if ( instance->buffer[index] == character )
			return index;

	return -1;
}

static U8String Substring(const U8String* instance, SizeType startIndex, SizeType length)
{
	SizeType count	= length;
	Char8* buffer	= CAST( Char8*, FAllocator.Allocate( sizeof(Char8) * ( count + 1 ) ) );

	buffer[count] = '\0';

	FMemory.Copy( instance->buffer + startIndex, buffer, sizeof(Char8) * count );

	U8String ret = { buffer, count };
	return ret;
}

static DynamicArray Split(const U8String* instance, Char8 separator)
{
	DynamicArray ret = FDynamicArray.Construct( 0, sizeof(U8String) );

	SizeType startIndex = 0;
	for (SizeType index = 0; index < instance->count; index++)
		if ( instance->buffer[index] == separator )
		{
			U8String substring = FU8StringExtension.Substring( instance, startIndex, index - startIndex );
			FDynamicArray.Add( &ret, &substring );

			startIndex = index + 1;
		}

	U8String substring = FU8StringExtension.Substring( instance, startIndex, instance->count - startIndex );
	FDynamicArray.Add( &ret, &substring );

	return ret;
}

static U8String Add(const U8String* leftHandSide, const U8String* rightHandSide)
{
	SizeType count	= leftHandSide->count + rightHandSide->count;
	Char8* buffer	= CAST( Char8*, FAllocator.Allocate( sizeof(Char8) * ( count + 1 ) ) );

	buffer[count] = '\0';

	FMemory.Copy( leftHandSide->buffer, buffer, sizeof(Char8) * leftHandSide->count );
	FMemory.Copy( rightHandSide->buffer, buffer + leftHandSide->count, sizeof(Char8) * rightHandSide->count );

	U8String ret = { buffer, count };
	return ret;
}

static U8String AddSC(const U8String* instance, Char8 character)
{
	U8String rhs = { &character, 1 };

	return FU8StringExtension.Add( instance, &rhs );
}

static U8String AddCS(Char8 character, const U8String* instance)
{
	U8String lhs = { &character, 1 };

	return FU8StringExtension.Add( &lhs, instance );
}

static Bool Equal(const U8String* leftHandSide, const U8String* rightHandSide)
{
	if ( leftHandSide->count != rightHandSide->count )
		return False;

	if (
		!FMemory.Equal(
			leftHandSide->buffer,
			rightHandSide->buffer,
			sizeof(Char8) * leftHandSide->count
		)
		)
		return False;

	return True;
}


struct FU8StringExtension FU8StringExtension =
{
	IndexOf,
	LastIndexOf,
	Substring,
	Split,
	Add,
	AddSC,
	AddCS,
	Equal,
};
