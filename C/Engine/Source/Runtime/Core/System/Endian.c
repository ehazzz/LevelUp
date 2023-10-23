#include "Endian.h"



static Bool IsLittleEndianCPU()
{
	union
	{
		Int32 integer;
		Char8 string[4];
	} value;

	value.integer = 0x12345678;

	return value.string[0] == 0x78;
}

static Bool IsBigEndianCPU()
{
	return !FEndian.IsLittleEndianCPU();
}

static UInt16 Swap16(UInt16 value)
{
	return
		( ( value & 0xFF00 ) >> 8 ) |
		( ( value & 0x00FF ) << 8 );
}

static UInt32 Swap32(UInt32 value)
{
	return
		( ( value & 0xFF000000 ) >> 24 ) |
		( ( value & 0x00FF0000 ) >>  8 ) |
		( ( value & 0x0000FF00 ) <<  8 ) |
		( ( value & 0x000000FF ) << 24 );
}

static UInt64 Swap64(UInt64 value)
{
	return
		( ( value & 0xFF00000000000000 ) >> 56 ) |
		( ( value & 0x00FF000000000000 ) >> 40 ) |
		( ( value & 0x0000FF0000000000 ) >> 24 ) |
		( ( value & 0x000000FF00000000 ) >>  8 ) |

		( ( value & 0x00000000FF000000 ) <<  8 ) |
		( ( value & 0x0000000000FF0000 ) << 24 ) |
		( ( value & 0x000000000000FF00 ) << 40 ) |
		( ( value & 0x00000000000000FF ) << 56 );
}


struct FEndian FEndian =
{
	IsLittleEndianCPU,
	IsBigEndianCPU,
	Swap16,
	Swap32,
	Swap64,
};