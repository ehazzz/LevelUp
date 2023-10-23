#pragma once

#include "../Base.h"



struct FAllocator
{
	Void*(*Allocate)(SizeType byteCount);
	Void(*Release)(Void* pointer);
};


extern ENGINE_API struct FAllocator FAllocator;