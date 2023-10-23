#pragma once

#include "File.h"
#include "Path.h"



typedef enum
{
	DirectoryEntrySearchOption_File			= 1 << 0,
	DirectoryEntrySearchOption_Directory	= 1 << 1,
}
DirectoryEntrySearchOption;


struct FDirectory
{
	Void(*Copy)(const U8String* from, const U8String* to);
	Void(*Create)(const U8String* path);
	Void(*Delete)(const U8String* path);
	Bool(*Exist)(const U8String* path);
	Void(*Move)(const U8String* from, const U8String* to);

	/**
	* @remark
	* The return that the container's datatype is U8String
	*/
	DynamicArray(*EnumEntries)(const U8String* path, DirectoryEntrySearchOption option);
};


extern ENGINE_API struct FDirectory FDirectory;