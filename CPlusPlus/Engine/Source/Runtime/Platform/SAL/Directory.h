#pragma once

#include "File.h"
#include "Path.h"



namespace Engine
{
	class ENGINE_API Directory
	{
	public:
		enum class SearchOption
		{
			File		= 1 << 0,
			Directory	= 1 << 1,
		};
		CLASS_ENUMCLASS_FLAGS(SearchOption);


		static Void Copy(const U8String& from, const U8String& to);
		static Void Create(const U8String& path);
		static Void Delete(const U8String& path);
		static Bool Exist(const U8String& path);
		static Void Move(const U8String& from, const U8String& to);

		static DynamicArray<U8String> EnumEntries(const U8String& path, SearchOption option);
	};
}