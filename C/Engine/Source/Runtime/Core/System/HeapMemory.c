#include "HeapMemory.h"



static HeapMemory Construct(SizeType byteCount)
{
	HeapMemory ret = { FAllocator.Allocate(byteCount), byteCount };
	return ret;
}

static Void Destruct(HeapMemory* instance)
{
	FAllocator.Release( instance->buffer );
}


struct FHeapMemory FHeapMemory =
{
	Construct,
	Destruct,
};