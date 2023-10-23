#pragma once

#include "../Base.h"



struct FEndian
{
	Bool(*IsLittleEndianCPU)();
	Bool(*IsBigEndianCPU)();

	UInt16(*Swap16)(UInt16 value);
	UInt32(*Swap32)(UInt32 value);
	UInt64(*Swap64)(UInt64 value);
};


extern ENGINE_API struct FEndian FEndian;