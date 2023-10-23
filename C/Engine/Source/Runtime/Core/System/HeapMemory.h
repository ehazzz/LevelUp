#pragma once

#include "Allocator.h"



typedef struct
{
	Void*		buffer;
	SizeType	byteCount;
}
HeapMemory;


struct FHeapMemory
{
	HeapMemory(*Construct)(SizeType byteCount);
	Void(*Destruct)(HeapMemory* instance);
};


extern ENGINE_API struct FHeapMemory FHeapMemory;