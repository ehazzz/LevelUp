#pragma once

#include "StringForward.h"



struct U32String
{
	Char32*		buffer;
	SizeType	count;
};


struct FU32String
{
	U32String(*Construct)(const Char32* utf32);
	Void(*Destruct)(U32String* instance);


	U8String(*ToU8String)(const U32String* instance);
	U16String(*ToU16String)(const U32String* instance);
};


extern ENGINE_API struct FU32String FU32String;