#pragma once

#include "StringForward.h"



struct U8String
{
	Char8*		buffer;
	SizeType	count;
};


struct FU8String
{
	U8String(*Construct)(const Char8* utf8);
	Void(*Destruct)(U8String* instance);


	U16String(*ToU16String)(const U8String* instance);
	U32String(*ToU32String)(const U8String* instance);
};


extern ENGINE_API struct FU8String FU8String;