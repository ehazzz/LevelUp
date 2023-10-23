#pragma once

#include "../../Core.h"



namespace Engine
{
	class ENGINE_API File
	{
	public:
		enum class OpenOption
		{
			Read,
			Write,
		};


		enum class SeekOption
		{
			Begin,
			Current,
			End,
		};


		NONCOPYABLE(File);


		File(const U8String& path, OpenOption option);
		~File();


		Void Seek(SeekOption option, Int64 offset);

		Void Read(Void* buffer, SizeType byteCount);
		Void Write(const Void* buffer, SizeType byteCount);


		static Void Copy(const U8String& from, const U8String& to);
		static Void Delete(const U8String& path);
		static Bool Exist(const U8String& path);
		static SizeType GetSize(const U8String& path);
		static Void Move(const U8String& from, const U8String& to);

		static HeapMemory ReadAllBytes(const U8String& path);
		static Void WriteAllBytes(const U8String& path, const Void* buffer, SizeType byteCount);
	private:
		Void* handle;
	};
}