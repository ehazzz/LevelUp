#pragma once

#include "../Mathematics/Math.h"

#include "../System/Allocator.h"



namespace Engine
{
	template<typename T>
	class DynamicArray
	{
	public:
		DynamicArray();
		DynamicArray(SizeType capacity);
		DynamicArray(const DynamicArray<T>& other);
		DynamicArray(DynamicArray<T>&& other);
		~DynamicArray();


		Void Add(const T& element);
		Void Add(T&& element);
		Void InsertAt(SizeType index, const T& element);
		Void InsertAt(SizeType index, T&& element);
		Void RemoveAt(SizeType index);

		Void Clear();


		T* GetBuffer();
		const T* GetBuffer() const;
		SizeType GetCount() const;
		SizeType GetCapacity() const;


		DynamicArray<T>& operator=(const DynamicArray<T>& other);
		DynamicArray<T>& operator=(DynamicArray<T>&& other);

		T& operator[](SizeType index);
		const T& operator[](SizeType index) const;
	public:
		T* begin();
		T* end();

		const T* begin() const;
		const T* end() const;
	private:
		Void CopyFrom(const DynamicArray<T>& instance);
		Void Expand();
	private:
		T* buffer;

		SizeType count;
		SizeType capacity;
	};


#pragma region Details
	template<typename T>
	DynamicArray<T>::DynamicArray()
		: buffer(nullptr), count(0), capacity(0)
	{

	}

	template<typename T>
	DynamicArray<T>::DynamicArray(SizeType capacity)
		: buffer( Allocator::Allocate<T>(capacity) ), count(0), capacity(capacity)
	{

	}

	template<typename T>
	DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
	{
		CopyFrom(other);
	}

	template<typename T>
	DynamicArray<T>::DynamicArray(DynamicArray<T>&& other)
	{
		buffer		= other.buffer;
		count		= other.count;
		capacity	= other.capacity;

		other.buffer = nullptr;
	}

	template<typename T>
	DynamicArray<T>::~DynamicArray()
	{
		Allocator::Release(buffer);
	}

	template<typename T>
	Void DynamicArray<T>::Add(const T& element)
	{
		if ( count == capacity )
			Expand();

		buffer[ count++ ] = element;
	}

	template<typename T>
	Void DynamicArray<T>::Add(T&& element)
	{
		if ( count == capacity )
			Expand();

		buffer[ count++ ] = Forward<T>(element);
	}

	template<typename T>
	Void DynamicArray<T>::InsertAt(SizeType index, const T& element)
	{
		if ( count == capacity )
			Expand();

		if ( index >= count )
			return;

		count++;
		for ( SizeType iteration = count - 1; iteration > index; iteration-- )
			buffer[iteration] = Move( buffer[ iteration - 1 ] );

		buffer[index] = element;
	}

	template<typename T>
	Void DynamicArray<T>::InsertAt(SizeType index, T&& element)
	{
		if ( count == capacity )
			Expand();

		if ( index >= count )
			return;

		count++;
		for ( SizeType iteration = count - 1; iteration > index; iteration-- )
			buffer[iteration] = Move( buffer[ iteration - 1 ] );

		buffer[index] = Forward<T>(element);
	}

	template<typename T>
	Void DynamicArray<T>::RemoveAt(SizeType index)
	{
		if ( index >= count )
			return;

		for ( SizeType iteraton = index + 1; iteraton < count; index++ )
			buffer[ iteraton - 1 ] = Move( buffer[iteraton] );

		count--;
	}

	template<typename T>
	Void DynamicArray<T>::Clear()
	{
		count = 0;
	}

	template<typename T>
	T* DynamicArray<T>::GetBuffer()
	{
		return buffer;
	}

	template<typename T>
	const T* DynamicArray<T>::GetBuffer() const
	{
		return buffer;
	}

	template<typename T>
	SizeType DynamicArray<T>::GetCount() const
	{
		return count;
	}

	template<typename T>
	SizeType DynamicArray<T>::GetCapacity() const
	{
		return capacity;
	}

	template<typename T>
	DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
	{
		this->~DynamicArray();
		CopyFrom(other);

		return *this;
	}

	template<typename T>
	DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other)
	{
		Swap( &buffer,		&other.buffer	);
		Swap( &count,		&other.count	);
		Swap( &capacity,	&other.capacity	);

		return *this;
	}

	template<typename T>
	T& DynamicArray<T>::operator[](SizeType index)
	{
		return buffer[index];
	}

	template<typename T>
	const T& DynamicArray<T>::operator[](SizeType index) const
	{
		return buffer[index];
	}

	template<typename T>
	T* DynamicArray<T>::begin()
	{
		return buffer;
	}

	template<typename T>
	T* DynamicArray<T>::end()
	{
		return buffer + count;
	}

	template<typename T>
	const T* DynamicArray<T>::begin() const
	{
		return buffer;
	}

	template<typename T>
	const T* DynamicArray<T>::end() const
	{
		return buffer + count;
	}

	template<typename T>
	Void DynamicArray<T>::CopyFrom(const DynamicArray<T>& instance)
	{
		SizeType size = instance.count;

		buffer		= Allocator::Allocate<T>(size);
		count		= size;
		capacity	= size;

		for ( SizeType index = 0; index < size; index++ )
			buffer[index] = instance[index];
	}

	template<typename T>
	Void DynamicArray<T>::Expand()
	{
		constexpr SizeType DefaultCapacity	= 8;
		constexpr RealType Scale			= 1.5;
		

		if ( capacity < DefaultCapacity )
			capacity = DefaultCapacity;
		else
			capacity *= Scale;

		T* temporary = Allocator::Allocate<T>(capacity);
		for ( SizeType index = 0; index < count; index++ )
			temporary[index] = Move( buffer[index] );

		this->~DynamicArray();
		buffer = temporary;
	}
#pragma endregion
}