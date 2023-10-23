#pragma once

#include "U8String.h"

#include "../Container/DynamicArray.h"



typedef U8String U8String;


struct FU8StringExtension
{
	Int32(*IndexOf)(const U8String* instance, Char8 character);
	Int32(*LastIndexOf)(const U8String* instance, Char8 character);

	U8String(*Substring)(const U8String* instance, SizeType startIndex, SizeType length);
	/**
	* @remark
	* The return that the container's datatype is U8String
	*/
	DynamicArray(*Split)(const U8String* instance, Char8 separator);


	U8String(*Add)(const U8String* leftHandSide, const U8String* rightHandSide);
	U8String(*AddSC)(const U8String* instance, Char8 character);
	U8String(*AddCS)(Char8 character, const U8String* instance);

	Bool(*Equal)(const U8String* leftHandSide, const U8String* rightHandSide);
};


extern ENGINE_API struct FU8StringExtension FU8StringExtension;