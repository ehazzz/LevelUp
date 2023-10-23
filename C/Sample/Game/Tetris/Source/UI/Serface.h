#pragma once

#include "Engine.h"



typedef struct
{
	Void* buffer;

	SizeType width;
	SizeType height;
	SizeType elementCount;
	RealType aspect;
	
	SizeType datatypeSize;
}
Serface;


struct FSerface
{
	Serface(*Construct)(SizeType width, SizeType height, SizeType datatypeSize);
	Void(*Destruct)(Serface* instance);


	Void(*Clear)(Serface* instance, const Void* value);

	Void(*SetValue)(Serface* instance, SizeType x, SizeType y, const Void* value);
	const Void*(*GetValue)(const Serface* instance, SizeType x, SizeType y);
};

extern ENGINE_API struct FSerface FSerface;