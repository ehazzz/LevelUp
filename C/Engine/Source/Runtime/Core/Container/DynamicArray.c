#include "DynamicArray.h"



static Void Expand(DynamicArray* instance)
{
	const SizeType DefaultCapacity	= 8;
	const RealType Scale			= 1.5;


	if ( instance->capacity < DefaultCapacity )
		instance->capacity = DefaultCapacity;
	else
		instance->capacity *= Scale;
	
	Void* buffer = FAllocator.Allocate( instance->datatypeSize * instance->capacity );
	FMemory.Copy(
		instance->buffer,
		buffer,
		instance->datatypeSize * instance->count
	);

	FDynamicArray.Destruct(instance);
	instance->buffer = buffer;
}


static DynamicArray Construct(SizeType capacity, SizeType datatypeSize)
{
	DynamicArray ret;
	ret.buffer			= FAllocator.Allocate( datatypeSize * capacity );
	ret.count			= 0;
	ret.capacity		= capacity;
	ret.datatypeSize	= datatypeSize;

	return ret;
}

static Void Destruct(DynamicArray* instance)
{
	FAllocator.Release( instance->buffer );
}

static Void Add(DynamicArray* instance, Void* element)
{
	if ( instance->count == instance->capacity )
		Expand(instance);

	FMemory.Copy(
		element,
		FDynamicArray.IndexAt( instance, instance->count++ ),
		instance->datatypeSize
	);
}

static Void InsertAt(DynamicArray* instance, SizeType index, Void* element)
{
	if ( index >= instance->count )
		return;

	if ( instance->count == instance->capacity )
		Expand(instance);

	instance->count++;
	for ( SizeType iteration = instance->count - 1; iteration > index; iteration-- )
		FMemory.Copy(
			FDynamicArray.IndexAt( instance, iteration - 1 ),
			FDynamicArray.IndexAt(instance, iteration),
			instance->datatypeSize
		);

	FMemory.Copy(
		element,
		FDynamicArray.IndexAt(instance, index),
		instance->datatypeSize
	);
}

static Void RemoveAt(DynamicArray* instance, SizeType index)
{
	if ( index >= instance->count )
		return;

	for ( SizeType iteration = index + 1; iteration < instance->count; index++ )
		FMemory.Copy(
			FDynamicArray.IndexAt(instance, iteration),
			FDynamicArray.IndexAt( instance, iteration - 1 ),
			instance->datatypeSize
		);

	instance->count--;
}

static Void Clear(DynamicArray* instance)
{
	instance->count = 0;
}

static Void* IndexAt(DynamicArray* instance, SizeType index)
{
	return CAST( Byte*, instance->buffer ) + instance->datatypeSize * index;
}


struct FDynamicArray FDynamicArray =
{
	Construct,
	Destruct,
	Add,
	InsertAt,
	RemoveAt,
	Clear,
	IndexAt,
};