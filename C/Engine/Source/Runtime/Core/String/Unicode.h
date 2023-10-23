#pragma once

#include "../Base.h"



struct FUnicode
{
	SizeType(*GetUTF8ElementCount)(const Char8* inUTF8, Char8 terminatedCharacter);
	SizeType(*GetUTF16ElementCount)(const Char16* inUTF16, Char16 terminatedCharacter);
	SizeType(*GetUTF32ElementCount)(const Char32* inUTF32, Char32 terminatedCharacter);

	SizeType(*GetCodePointCountFromUTF8Elements)(const Char8* utf8, SizeType count);
	SizeType(*GetCodePointCountFromUTF16Elements)(const Char16* utf16, SizeType count);

	SizeType(*GetUTF8ElementCountFromCodePoints)(const CodePoint* in, SizeType count);
	SizeType(*GetUTF16ElementCountFromCodePoints)(const CodePoint* in, SizeType count);

	/**
	* Convert UTF8/UTF16 encoding sequence to codepoint
	* 
	* 
	* @param outputResult	As a output param and calculated result(codepoint) will be placed in
	* 
	* @return Indicate how many UTF8/UTF16 elements had been used
	*/
	SizeType(*FromUTF8)(CodePoint* outputResult, const Char8* inUTF8Sequence);
	SizeType(*FromUTF16)(CodePoint* outputResult, const Char16* inUTF16Sequence);
	/**
	* Convert codepoint to UTF8/UTF16 encoding sequence
	* 
	* 
	* @param outputResult	As a ouput param(a pointer point to allocated heap/stack memory) and calculated result(UTF8/UTF16 encoding sequence) will be placed in
	* 
	* @return Indicate a codepoint will be converted to how many UTF8/UTF16 elements
	*/
	SizeType(*ToUTF8)(Char8* outputResult, CodePoint codepoint);
	SizeType(*ToUTF16)(Char16* outputResult, CodePoint codepoint);
};


extern ENGINE_API struct FUnicode FUnicode;