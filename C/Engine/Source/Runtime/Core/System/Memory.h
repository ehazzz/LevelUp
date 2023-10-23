#pragma once

#include "../Base.h"



struct FMemory
{
	Void(*Clear)(Void* buffer, SizeType byteCount);
	Void(*Copy)(const Void* from, Void* to, SizeType byteCount);
	Bool(*Equal)(const Void* bufferA, const Void* bufferB, SizeType byteCount);
};


extern ENGINE_API struct FMemory FMemory;