#pragma once

#include "../Base.h"



namespace Engine
{
	ENGINE_API Bool IsLittleEndianCPU();
	ENGINE_API Bool IsBigEndianCPU();

	ENGINE_API UInt16 Swap16(UInt16 value);
	ENGINE_API UInt32 Swap32(UInt32 value);
	ENGINE_API UInt64 Swap64(UInt64 value);
}