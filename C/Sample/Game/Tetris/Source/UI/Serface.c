#include "Serface.h"



static Serface Construct(SizeType width, SizeType height, SizeType datatypeSize)
{
	SizeType elementCount = width * height;

	if ( elementCount == 0 )
	{
		Serface ret;
		FMemory.Clear( &ret, sizeof(Serface) );

		return ret;
	}

	Serface ret;
	ret.buffer			= FAllocator.Allocate( datatypeSize * elementCount );
	ret.width			= width;
	ret.height			= height;
	ret.elementCount	= elementCount;
	ret.aspect			= CAST(RealType, width) / height;
	ret.datatypeSize	= datatypeSize;

	return ret;
}

static Void Destruct(Serface* instance)
{
	if ( !instance )
		return;

	FAllocator.Release( instance->buffer );
}

static Void Clear(Serface* instance, const Void* value)
{
	for ( SizeType index = 0; index < instance->elementCount; index++ )
		FMemory.Copy(
			value,
			CAST( Byte*, instance->buffer ) + instance->datatypeSize * index,
			instance->datatypeSize
		);
}

static Void SetValue(Serface* instance, SizeType x, SizeType y, const Void* value)
{
	FMemory.Copy(
		value,
		CAST( Byte*, instance->buffer ) + instance->datatypeSize * ( instance->width * y + x ),
		instance->datatypeSize
	);
}

static const Void* GetValue(const Serface* instance, SizeType x, SizeType y)
{
	return CAST( Byte*, instance->buffer ) + instance->datatypeSize * ( instance->width * y + x );
}


struct FSerface FSerface = 
{
	Construct,
	Destruct,
	Clear,
	SetValue,
	GetValue,
};
