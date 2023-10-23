#pragma once

#include "Type.h"



namespace Engine
{
	class Memory
	{
	public:
		static Void Clear(Void* buffer, SizeType byteCount);
		static Void Copy(const Void* from, Void* to, SizeType byteCount);
		static Bool Equal(const Void* bufferA, const Void* bufferB, SizeType byteCount);
	};
}