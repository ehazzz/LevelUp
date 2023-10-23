#include "Unicode.h"



static SizeType GetUTF8ElementCount(const Char8* inUTF8, Char8 terminatedCharacter)
{
	SizeType ret = 0;
	while ( inUTF8[ret] != terminatedCharacter ) ret++;

	return ret;
}

static SizeType GetUTF16ElementCount(const Char16* inUTF16, Char16 terminatedCharacter)
{
	SizeType ret = 0;
	while ( inUTF16[ret] != terminatedCharacter ) ret++;

	return ret;
}

static SizeType GetUTF32ElementCount(const Char32* inUTF32, Char32 terminatedCharacter)
{
	SizeType ret = 0;
	while ( inUTF32[ret] != terminatedCharacter ) ret++;

	return ret;
}

static SizeType GetCodePointCountFromUTF8Elements(const Char8* utf8, SizeType count)
{
	SizeType ret = 0;
	for (SizeType offset = 0; offset < count;)
	{
		Char8 value = utf8[offset];

		if ( ( value & 0x80 ) == 0 )
			offset += 1;
		else if ( ( value & 0xE0 ) == 0xC0 )
			offset += 2;
		else if ( ( value & 0xF0 ) == 0xE0 )
			offset += 3;
		else if ( ( value & 0xF8 ) == 0xF0 )
			offset += 4;

		ret++;
	}

	return ret;
}

static SizeType GetCodePointCountFromUTF16Elements(const Char16* utf16, SizeType count)
{
	SizeType ret = 0;
	for (SizeType offset = 0; offset < count;)
	{
		Char16 value = utf16[offset];

		if ( ( value & 0xFC00 ) == 0xDC00 )
			offset += 2;
		else
			offset += 1;

		ret++;
	}

	return ret;
}

static SizeType GetUTF8ElementCountFromCodePoints(const CodePoint* in, SizeType count)
{
	SizeType ret = 0;
	for (SizeType index = 0; index < count; index++)
	{
		CodePoint codepoint = in[index];

		if ( codepoint <= 0x7F )
			ret += 1;
		else if ( codepoint <= 0x7FF )
			ret += 2;
		else if ( codepoint <= 0xFFFF )
			ret += 3;
		else if ( codepoint <= 0x10FFFF )
			ret += 4;
	}

	return ret;
}

static SizeType GetUTF16ElementCountFromCodePoints(const CodePoint* in, SizeType count)
{
	SizeType ret = 0;
	for (SizeType index = 0; index < count; index++)
	{
		CodePoint codepoint = in[index];

		if ( codepoint <= 0xFFFF )
			ret += 1;
		else if ( codepoint <= 0x10FFFF )
			ret += 2;
	}

	return ret;
}

static SizeType FromUTF8(CodePoint* outputResult, const Char8* inUTF8Sequence)
{
	SizeType ret = 0;

	CodePoint codepoint = 0;

	const Char8* c0 = inUTF8Sequence + 0;
	const Char8* c1 = inUTF8Sequence + 1;
	const Char8* c2 = inUTF8Sequence + 2;
	const Char8* c3 = inUTF8Sequence + 3;

	if ( ( *c0 & 0x80 ) == 0 )
	{
		codepoint = *c0;

		ret = 1;
	}
	else if ( ( *c0 & 0xE0 ) == 0xC0 )
	{
		codepoint |= ( *c0 & 0x1F ) << 6;
		codepoint |= ( *c1 & 0x3F ) << 0;

		ret = 2;
	}
	else if ( ( *c0 & 0xF0 ) == 0xE0 )
	{
		codepoint |= ( *c0 & 0x0F ) << 12;
		codepoint |= ( *c1 & 0x3F ) <<  6;
		codepoint |= ( *c2 & 0x3F ) <<  0;

		ret = 3;
	}
	else if ( ( *c0 & 0xF8 ) == 0xF0 )
	{
		codepoint |= ( *c0 & 0x07 ) << 18;
		codepoint |= ( *c1 & 0x3F ) << 12;
		codepoint |= ( *c2 & 0x3F ) <<  6;
		codepoint |= ( *c3 & 0x3F ) <<  0;

		ret = 4;
	}

	if (outputResult)
		*outputResult = codepoint;

	return ret;
}

static SizeType FromUTF16(CodePoint* outputResult, const Char16* inUTF16Sequence)
{
	SizeType ret = 0;

	CodePoint codepoint = 0;

	const Char16* c0 = inUTF16Sequence + 0;
	const Char16* c1 = inUTF16Sequence + 1;

	if ( ( *c0 & 0xFC00 ) == 0xD800 )
	{
		codepoint |= ( *c0 & 0x3FF ) << 10;
		codepoint |= ( *c1 & 0x3FF ) <<  0;

		codepoint += 0x10000;

		ret = 2;
	}
	else
	{
		codepoint = *c0;

		ret = 1;
	}

	if (outputResult)
		*outputResult = codepoint;

	return ret;
}

static SizeType ToUTF8(Char8* outputResult, CodePoint codepoint)
{
	SizeType ret = 0;

	Char8* c0 = outputResult + 0;
	Char8* c1 = outputResult + 1;
	Char8* c2 = outputResult + 2;
	Char8* c3 = outputResult + 3;

	if ( codepoint <= 0x7F )
	{
		*c0 = codepoint;

		ret = 1;
	}
	else if ( codepoint <= 0x7FF )
	{
		*c0 = ( ( codepoint >> 6 ) & 0x1F ) | 0xC0;
		*c1 = ( ( codepoint >> 0 ) & 0x3F ) | 0x80;

		ret = 2;
	}
	else if ( codepoint <= 0xFFFF )
	{
		*c0 = ( ( codepoint >> 12 ) & 0x0F ) | 0xE0;
		*c1 = ( ( codepoint >>  6 ) & 0x3F ) | 0x80;
		*c2 = ( ( codepoint >>  0 ) & 0x3F ) | 0x80;

		ret = 3;
	}
	else if ( codepoint <= 0x10FFFF )
	{
		*c0 = ( ( codepoint >> 18 ) & 0x07 ) | 0xF0;
		*c1 = ( ( codepoint >> 12 ) & 0x3F ) | 0x80;
		*c2 = ( ( codepoint >>  6 ) & 0x3F ) | 0x80;
		*c3 = ( ( codepoint >>  0 ) & 0x3F ) | 0x80;

		ret = 4;
	}

	return ret;
}

static SizeType ToUTF16(Char16* outputResult, CodePoint codepoint)
{
	SizeType ret = 0;

	Char16* c0 = outputResult + 0;
	Char16* c1 = outputResult + 1;

	if ( codepoint <= 0xFFFF )
	{
		*c0 = codepoint;

		ret = 1;
	}
	else if ( codepoint <= 0x10FFFF )
	{
		codepoint -= 0x10000;

		*c0 = ( ( codepoint >> 10 ) & 0x3FF ) | 0xD800;
		*c1 = ( ( codepoint >>  0 ) & 0x3FF ) | 0xDC00;

		ret = 2;
	}

	return ret;
}


struct FUnicode FUnicode =
{
	GetUTF8ElementCount,
	GetUTF16ElementCount,
	GetUTF32ElementCount,
	GetCodePointCountFromUTF8Elements,
	GetCodePointCountFromUTF16Elements,
	GetUTF8ElementCountFromCodePoints,
	GetUTF16ElementCountFromCodePoints,
	FromUTF8,
	FromUTF16,
	ToUTF8,
	ToUTF16,
};
