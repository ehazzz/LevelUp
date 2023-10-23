#pragma once

#include "Unicode.h"



struct FStringConversion
{
	Bool(*TryParseBool)(Bool* outputResult, const Char8* inAsciiString);
	Bool(*TryParseInt32)(Int32* outputResult, const Char8* inAsciiString);
	Bool(*TryParseFloat32)(Float32* outputResult, const Char8* inAsciiString);

	/**
	* Convert Bool/Int32/Float32's value to a Ascii string representation with '\0' as terminated character
	* 
	* 
	* @param inBuffer	A pointer point to allocated heap/stack memory
	* @param inSize		Indicate how many Char8s could be used
	* 
	* @return True if successed otherwise False
	*/
	Bool(*TryConvertBool)(Char8* inBuffer, SizeType inSize, Bool value);
	Bool(*TryConvertInt32)(Char8* inBuffer, SizeType inSize, Int32 value);
	Bool(*TryConvertFloat32)(Char8* inBuffer, SizeType inSize, Float32 value);
};


extern ENGINE_API struct FStringConversion FStringConversion;