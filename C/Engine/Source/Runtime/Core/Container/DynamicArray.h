#pragma once

#include "../System/Allocator.h"
#include "../System/Memory.h"



typedef struct
{
	Void* buffer;

	SizeType count;
	SizeType capacity;
	SizeType datatypeSize;
}
DynamicArray;


struct FDynamicArray
{
	DynamicArray(*Construct)(SizeType capacity, SizeType datatypeSize);
	Void(*Destruct)(DynamicArray* instance);


	Void(*Add)(DynamicArray* instance, Void* element);
	Void(*InsertAt)(DynamicArray* instance, SizeType index, Void* element);
	Void(*RemoveAt)(DynamicArray* instance, SizeType index);

	Void(*Clear)(DynamicArray* instance);


	Void*(*IndexAt)(DynamicArray* instance, SizeType index);
};


extern ENGINE_API struct FDynamicArray FDynamicArray;