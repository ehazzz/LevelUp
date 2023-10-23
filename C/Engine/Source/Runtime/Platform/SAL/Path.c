#include "Path.h"



// Test if the last element of path is slash or back slash
static Bool IsNormalized(const U8String* path)
{
	Char8 last = path->buffer[ path->count - 1 ];

	return last == '/' || last == '\\';
}


static U8String Combine(const U8String* A, const U8String* B)
{
	SizeType length = A->count + B->count;

	Bool normalized;
	if ( A->count == 0 || IsNormalized(A) )
	{
		normalized	= True;
		length		+= 0;
	}
	else
	{
		normalized	= False;
		length		+= 1;
	}

	U8String ret = { CAST( Char8*, FAllocator.Allocate( sizeof(Char8) * ( length + 1 ) ) ), length };
	{
		SizeType offset = 0;

		FMemory.Copy( A->buffer, ret.buffer, sizeof(Char8) * A->count );
		offset += A->count;

		if ( !normalized )
		{
			// insert slash
			ret.buffer[ A->count ] = '/';

			offset += 1;
		}

		FMemory.Copy( B->buffer, ret.buffer + offset, sizeof(Char8) * ( B->count + 1 ) );
	}

	return ret;
}


struct FPath FPath = 
{
	Combine,
};
