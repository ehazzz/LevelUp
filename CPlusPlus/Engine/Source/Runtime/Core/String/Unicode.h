#pragma once

#include "../Base.h"
#include "../Template.h"



namespace Engine
{
	enum class Encoding
	{
		UTF8,
		UTF16,
	};


	template<Encoding E>
	using UTFDeduceDataType = typename SelectNthFromParameterPack<static_cast<SizeType>(E), Char8, Char16>::Type;


	template<typename T> SizeType GetUTFElementCount(const T* inUTF, T terminatedCharacter = T());

	template<Encoding E, typename T = UTFDeduceDataType<E>> SizeType GetCodePointCountFromUTFElements(const T* inUTF, SizeType count);
	template<Encoding E> SizeType GetUTFElementCountFromCodePoints(const CodePoint* in, SizeType count);

	/**
	* Convert UTF encoding sequence to codepoint
	* 
	* 
	* @param outputResult	As a output parameter and calculated result(codepoint) will be placed in
	* 
	* @return Indicate how many UTF elements had been used from [inUTFSequence]
	*/
	template<Encoding E, typename T = UTFDeduceDataType<E>> SizeType FromUTF(CodePoint* outputResult, const T* inUTFSequence);
	/**
	* Convert codepoint to UTF encoding sequence
	* 
	* 
	* @param outputResult	As a ouput parameter(a pointer point to allocated heap/stack memory) and calculated result(UTF encoding sequence) will be placed in
	* 
	* @return Indicate a codepoint will be converted to how many UTF elements
	*/
	template<Encoding E, typename T = UTFDeduceDataType<E>> SizeType ToUTF(T* outputResult, CodePoint codepoint);


#pragma region Details
	template<typename T>
	SizeType GetUTFElementCount(const T* inUTF, T terminatedCharacter)
	{
		static_assert( IsCharacter<T>::Value );


		SizeType ret = 0;
		while ( inUTF[ret] != terminatedCharacter ) ret++;
		
		return ret;
	}

	template<Encoding E, typename T>
	SizeType GetCodePointCountFromUTFElements(const T* inUTF, SizeType count)
	{
		SizeType ret = 0;
		for ( SizeType offset = 0; offset < count; )
		{
			T value = inUTF[offset];

			if constexpr ( E == Encoding::UTF8 )
			{
				if ( ( value & 0x80 ) == 0 )
					offset += 1;
				else if ( ( value & 0xE0 ) == 0xC0 )
					offset += 2;
				else if ( ( value & 0xF0 ) == 0xE0 )
					offset += 3;
				else if ( ( value & 0xF8 ) == 0xF0 )
					offset += 4;
			}

			if constexpr ( E == Encoding::UTF16 )
			{
				if ( ( value & 0xFC00 ) == 0xD800 )
					offset += 2;
				else
					offset += 1;
			}

			ret++;
		}

		return ret;
	}

	template<Encoding E>
	SizeType GetUTFElementCountFromCodePoints(const CodePoint* in, SizeType count)
	{
		SizeType ret = 0;
		for ( SizeType index = 0; index < count; index++ )
		{
			CodePoint codepoint = in[index];

			if constexpr ( E == Encoding::UTF8 )
			{
				if ( codepoint <= 0x7F )
					ret += 1;
				else if ( codepoint <= 0x3FF )
					ret += 2;
				else if ( codepoint <= 0xFFFF )
					ret += 3;
				else if ( codepoint <= 0x10FFFF )
					ret += 4;
			}

			if constexpr ( E == Encoding::UTF16 )
			{
				if ( codepoint <= 0xFFFF )
					ret += 1;
				else if ( codepoint <= 0x10FFFF )
					ret += 2;
			}
		}

		return ret;
	}

	template<Encoding E, typename T>
	SizeType FromUTF(CodePoint* outputResult, const T* inUTFSequence)
	{
		SizeType ret		= 0;
		SizeType codepoint	= 0;

		auto c0 = inUTFSequence + 0;
		auto c1 = inUTFSequence + 1;
		auto c2 = inUTFSequence + 2;
		auto c3 = inUTFSequence + 3;

		if constexpr ( E == Encoding::UTF8 )
		{
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
		}
	
		if constexpr ( E == Encoding::UTF16 )
		{
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
		}

		if (outputResult)
			*outputResult = codepoint;

		return ret;
	}

	template<Encoding E, typename T>
	SizeType ToUTF(T* outputResult, CodePoint codepoint)
	{
		SizeType ret = 0;

		auto c0 = outputResult + 0;
		auto c1 = outputResult + 1;
		auto c2 = outputResult + 2;
		auto c3 = outputResult + 3;

		if constexpr ( E == Encoding::UTF8 )
		{
			if ( codepoint <= 0x7F )
			{
				*c0 = codepoint;

				ret = 1;
			}
			else if ( codepoint <= 0x3FF )
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
		}

		if constexpr ( E == Encoding::UTF16 )
		{
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
		}

		return ret;
	}
#pragma endregion
}