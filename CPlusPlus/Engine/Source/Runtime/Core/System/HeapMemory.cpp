#include "HeapMemory.h"



namespace Engine
{
	HeapMemory::HeapMemory()
		: buffer(nullptr), byteCount(0)
	{

	}

	HeapMemory::HeapMemory(SizeType preallocated)
		: buffer( Allocator::Allocate<Byte>(preallocated) ), byteCount(preallocated)
	{

	}

	HeapMemory::HeapMemory(Void* buffer, SizeType byteCount)
		: buffer(buffer), byteCount(byteCount)
	{

	}

	HeapMemory::HeapMemory(const HeapMemory& other)
	{
		CopyFrom(other);
	}

	HeapMemory::HeapMemory(HeapMemory&& other)
	{
		buffer		= other.buffer;
		byteCount	= other.byteCount;

		other.buffer = nullptr;
	}

	HeapMemory::~HeapMemory()
	{
		Allocator::Release( reinterpret_cast<Byte*>(buffer) );
	}

	Void* HeapMemory::GetBuffer()
	{
		return buffer;
	}

	const Void* HeapMemory::GetBuffer() const
	{
		return buffer;
	}

	SizeType HeapMemory::GetByteCount() const
	{
		return byteCount;
	}

	HeapMemory& HeapMemory::operator=(const HeapMemory& other)
	{
		this->~HeapMemory();
		CopyFrom(other);

		return *this;
	}

	HeapMemory& HeapMemory::operator=(HeapMemory&& other)
	{
		Swap( &buffer,		&other.buffer	 );
		Swap( &byteCount,	&other.byteCount );

		return *this;
	}

	Void HeapMemory::CopyFrom(const HeapMemory& instance)
	{
		SizeType size = instance.byteCount;

		buffer		= Allocator::Allocate<Byte>(size);
		byteCount	= size;

		Memory::Copy(
			instance.buffer,
			buffer,
			size
		);
	}
}