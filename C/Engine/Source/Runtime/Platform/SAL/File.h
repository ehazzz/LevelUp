#pragma once

#include "../../Core.h"



typedef enum
{
	FileOpenOption_Read,
	FileOpenOption_Write,
}
FileOpenOption;


typedef enum
{
	FileSeekOption_Begin,
	FileSeekOption_Current,
	FileSeekOption_End,
}
FileSeekOption;


typedef struct
{
	Void* handle;
}
File;


struct FFile
{
	File(*Construct)(const U8String* path, FileOpenOption option);
	Void(*Destruct)(File* instance);


	Void(*Seek)(File* instance, FileSeekOption option, Int64 offset);

	Void(*Read)(File* instance, Void* buffer, SizeType byteCount);
	Void(*Write)(File* instance, const Void* buffer, SizeType byteCount);

	
	Void(*Copy)(const U8String* from, const U8String* to);
	Void(*Delete)(const U8String* path);
	Bool(*Exist)(const U8String* path);
	SizeType(*GetSize)(const U8String* path);
	Void(*Move)(const U8String* from, const U8String* to);

	HeapMemory(*ReadAllBytes)(const U8String* path);
	Void(*WriteAllBytes)(const U8String* path, const Void* buffer, SizeType byteCount);
};


extern ENGINE_API struct FFile FFile;