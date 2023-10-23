#pragma once

#include "StringForward.h"



struct U16String
{
	Char16*		buffer;
	SizeType	count;
};


struct FU16String
{
	U16String(*Construct)(const Char16* utf16);
	Void(*Destruct)(U16String* instance);


	U8String(*ToU8String)(const U16String* instance);
	U32String(*ToU32String)(const U16String* instance);
};


extern ENGINE_API struct FU16String FU16String;