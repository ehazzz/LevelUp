#pragma once

#include "../Container/DynamicArray.h"

#include "../System/Memory.h"

#include "Unicode.h"



namespace Engine
{
	template<typename T> class String;

	using U8String	= String<Char8>;	// Used to represent UTF8 encoding text
	using U16String = String<Char16>;	// Used to represent UTF16 encoding text
	using U32String = String<Char32>;	// Used to represent UTF32 encoding text


	ENGINE_API Bool TryParseBool(Bool* outputResult, const Char8* in);
	ENGINE_API Bool TryParseFloat32(Float32* outputResult, const Char8* in);
	ENGINE_API Bool TryParseInt32(Int32* outputResult, const Char8* in);

	ENGINE_API Bool TryConvertBool(Char8* inBuffer, SizeType inSize, Bool value);
	ENGINE_API Bool TryConvertFloat32(Char8* inBuffer, SizeType inSize, Float32 value);
	ENGINE_API Bool TryConvertInt32(Char8* inBuffer, SizeType inSize, Int32 value);


	template<typename T>
	class String
	{
	public:
		String();
		String(T* buffer, SizeType count);
		String(const T* inUTF);
		String(const String<T>& other);
		String(String<T>&& other);
		~String();


		/**
		* @remark
		* Return -1 if not found
		*/
		Int32 IndexOf(T character) const;
		Int32 LastIndexOf(T character) const;
		DynamicArray<String<T>> Split(T separator) const;
		String<T> Substring(SizeType startIndex, SizeType length) const;
		String<T> Substring(SizeType startIndex) const;


		T* GetBuffer();
		const T* GetBuffer() const;
		SizeType GetCount() const;


		String<T>& operator=(const String<T>& other);
		String<T>& operator=(String<T>&& other);

		T& operator[](SizeType index);
		const T& operator[](SizeType index) const;

		String<T> operator+(T character) const;
		String<T> operator+(const String<T>& other) const;
		friend String<T> operator+(T character, const String<T>& instance);

		String<T>& operator+=(T character);
		String<T>& operator+=(const String<T>& other);

		Bool operator==(const String<T>& other);
		Bool operator!=(const String<T>& other);

		operator T* ();
		operator const T* () const;

		operator U8String() const;
		operator U16String() const;
		operator U32String() const;
	private:
		Void CopyFrom(const String<T>& instance);
		template<Encoding E> typename SelectNthFromParameterPack<static_cast<SizeType>(E), U8String, U16String>::Type ConvertFromU32String() const;
	private:
		T*			buffer;
		SizeType	count;
	};


#pragma region Details
	template<typename T>
	String<T>::String()
		: buffer(nullptr), count(0)
	{

	}

	template<typename T>
	String<T>::String(T* buffer, SizeType count)
		: buffer(buffer), count(count)
	{

	}

	template<typename T>
	String<T>::String(const T* inUTF)
	{
		String<T> temporary( (T*)inUTF, GetUTFElementCount(inUTF) );
		CopyFrom(temporary);

		temporary.buffer = nullptr;
	}

	template<typename T>
	String<T>::String(const String<T>& other)
	{
		CopyFrom(other);
	}

	template<typename T>
	String<T>::String(String<T>&& other)
	{
		buffer	= other.buffer;
		count	= other.count;

		other.buffer = nullptr;
	}

	template<typename T>
	String<T>::~String()
	{
		Allocator::Release(buffer);
	}

	template<typename T>
	Int32 String<T>::IndexOf(T character) const
	{
		for ( SizeType index = 0; index < count; index++ )
			if ( buffer[index] == character )
				return index;

		return -1;
	}

	template<typename T>
	Int32 String<T>::LastIndexOf(T character) const
	{
		for( Int32 index = count - 1; index >= 0; index-- )
			if ( buffer[index] == character )
				return index;

		return -1;
	}

	template<typename T>
	DynamicArray<String<T>> String<T>::Split(T separator) const
	{
		DynamicArray<String<T>> ret;

		SizeType startIndex = 0;
		for( SizeType index = 0; index < count; index++ )
			if ( buffer[index] == separator )
			{
				ret.Add(
					Substring( startIndex, index - startIndex )
				);

				startIndex = index + 1;
			}

		ret.Add( Substring(startIndex) );

		return ret;
	}

	template<typename T>
	String<T> String<T>::Substring(SizeType startIndex, SizeType length) const
	{
		T* buffer = Allocator::Allocate<T>( length + 1 );

		Memory::Copy(
			this->buffer + startIndex,
			buffer,
			sizeof(T) * length
		);

		buffer[length] = '\0';

		return String<T>(buffer, length);
	}

	template<typename T>
	String<T> String<T>::Substring(SizeType startIndex) const
	{
		return Substring( startIndex, count - startIndex );
	}

	template<typename T>
	T* String<T>::GetBuffer()
	{
		return buffer;
	}

	template<typename T>
	const T* String<T>::GetBuffer() const
	{
		return buffer;
	}

	template<typename T>
	SizeType String<T>::GetCount() const
	{
		return count;
	}

	template<typename T>
	String<T>& String<T>::operator=(const String<T>& other)
	{
		this->~String();
		CopyFrom(other);

		return *this;
	}

	template<typename T>
	String<T>& String<T>::operator=(String<T>&& other)
	{
		Swap( &buffer,	&other.buffer );
		Swap( &count,	&other.count  );

		return *this;
	}

	template<typename T>
	T& String<T>::operator[](SizeType index)
	{
		return buffer[index];
	}

	template<typename T>
	const T& String<T>::operator[](SizeType index) const
	{
		return buffer[index];
	}

	template<typename T>
	String<T> String<T>::operator+(T character) const
	{
		String<T> rightHandSide( &character, 1 );

		auto ret				= *this + rightHandSide;
		rightHandSide.buffer	= nullptr;

		return ret;
	}

	template<typename T>
	String<T> String<T>::operator+(const String<T>& other) const
	{
		SizeType count	= this->count + other.count;
		T* buffer		= Allocator::Allocate<T>( count + 1 );

		Memory::Copy(
			this->buffer,
			buffer,
			sizeof(T) * this->count
		);

		Memory::Copy(
			other.buffer,
			buffer + this->count,
			sizeof(T) * other.count
		);

		buffer[count] = '\0';

		return String<T>(buffer, count);
	}

	template<typename T>
	String<T> operator+(T character, const String<T>& instance)
	{
		String<T> leftHandSide( &character, 1 );

		auto ret			= leftHandSide + *this;
		leftHandSide.buffer	= nullptr;

		return ret;
	}

	template<typename T>
	String<T>& String<T>::operator+=(T character)
	{
		*this = *this + character;

		return *this;
	}

	template<typename T>
	String<T>& String<T>::operator+=(const String<T>& other)
	{
		*this = *this + other;

		return *this;
	}

	template<typename T>
	Bool String<T>::operator==(const String<T>& other)
	{
		if ( count != other.count )
			return false;

		if (
			!Memory::Equal(
				buffer,
				other.buffer,
				sizeof(T) * count
			)
			)
			return false;

		return true;
	}

	template<typename T>
	Bool String<T>::operator!=(const String<T>& other)
	{
		return !operator==(other);
	}

	template<typename T>
	String<T>::operator T* ()
	{
		return buffer;
	}

	template<typename T>
	String<T>::operator const T* () const
	{
		return buffer;
	}

	template<typename T>
	String<T>::operator U8String() const
	{
		if constexpr ( TypesAreSame<T, Char16>::Value )
		{
			return static_cast<U32String>( *this );
		}

		if constexpr ( TypesAreSame<T, Char32>::Value )
		{
			/*
			SizeType count	= GetUTFElementCountFromCodePoints<Encoding::UTF8>( this->buffer, this->count );
			Char8* buffer	= Allocator::Allocate<Char8>( count + 1 );

			SizeType offset = 0;
			for (SizeType index = 0; index < this->count; index++)
				offset += ToUTF<Encoding::UTF8>(
					buffer + offset,
					this->buffer[index]
				);

			buffer[count] = 0x00;

			return U8String(buffer, count);
			*/

			return ConvertFromU32String<Encoding::UTF8>();
		}
	}

	template<typename T>
	String<T>::operator U16String() const
	{
		if constexpr ( TypesAreSame<T, Char8>::Value )
		{
			return static_cast<U32String>( *this );
		}

		if constexpr ( TypesAreSame<T, Char32>::Value )
		{
			/*
			SizeType count = GetUTFElementCountFromCodePoints<Encoding::UTF16>( this->buffer, this->count );
			Char16* buffer = Allocator::Allocate<Char16>( count + 1 );

			SizeType offset = 0;
			for (SizeType index = 0; index < this->count; index++)
				offset += ToUTF<Encoding::UTF16>(
					buffer + offset,
					this->buffer[index]
				);

			buffer[count] = 0x0000;

			return U16String(buffer, count);
			*/

			return ConvertFromU32String<Encoding::UTF16>();
		}
	}

	template<typename T>
	String<T>::operator U32String() const
	{
		constexpr auto E = TypesAreSame<T, Char8>::Value ? Encoding::UTF8 : Encoding::UTF16;

		SizeType count		= GetCodePointCountFromUTFElements<E>( this->buffer, this->count );
		CodePoint* buffer	= Allocator::Allocate<CodePoint>( count + 1 );

		SizeType offset = 0;
		for ( SizeType index = 0; index < count; index++ )
			offset += FromUTF<E>(
				buffer + index,
				this->buffer + offset
			);

		buffer[count] = '\0';

		return U32String(buffer, count);
	}

	template<typename T>
	Void String<T>::CopyFrom(const String<T>& instance)
	{
		SizeType size = instance.count;

		buffer	= Allocator::Allocate<T>( size + 1 );
		count	= size;

		Memory::Copy(
			instance.buffer,
			buffer,
			sizeof(T) * count
		);

		buffer[count] = '\0';
	}

	template<typename T>
	template<Encoding E>
	typename SelectNthFromParameterPack<static_cast<SizeType>(E), U8String, U16String>::Type String<T>::ConvertFromU32String() const
	{
		using ValueType = typename SelectNthFromParameterPack<static_cast<SizeType>(E), Char8, Char16>::Type;


		SizeType count	= GetUTFElementCountFromCodePoints<E>( this->buffer, this->count );
		auto buffer		= Allocator::Allocate<ValueType>( count + 1 );

		SizeType offset = 0;
		for ( SizeType index = 0; index < this->count; index++ )
			offset += ToUTF<E>(
				buffer + offset,
				this->buffer[index]
			);

		buffer[count] = '\0';

		return typename SelectNthFromParameterPack<static_cast<SizeType>(E), U8String, U16String>::Type(buffer, count);
	}
#pragma endregion
}