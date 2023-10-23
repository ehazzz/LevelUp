#include "String.h"



namespace Engine
{
	static const Char8* SkipWhitespace(const Char8* in)
	{
		while (
			*in == '\r' ||
			*in == '\n' ||
			*in == '\t' ||
			*in == ' '
			)
			in++;

		return in;
	}

	static Bool IsNumber(Char8 value)
	{
		return value >= '0' && value <= '9';
	}

	static Int32 FindDot(const Char8* in)
	{
		SizeType ret = 0;
		while (true)
		{
			if ( *in == 0x00 )
				break;

			if ( *in == '.' )
				return ret;

			in++;
			ret++;
		}

		return -1;
	}


	Bool TryParseBool(Bool* outputResult, const Char8* in)
	{
		in = SkipWhitespace(in);

		Bool parse;
		switch ( *in )
		{
		case 'f':
		case 'F':
			parse = false;
			break;
		case 't':
		case 'T':
			parse = true;
			break;
		default:
			return false;
		}

		if (outputResult)
			*outputResult = parse;

		return true;
	}

	Bool TryParseFloat32(Float32* outputResult, const Char8* in)
	{
		Int32 integerPart;
		if ( !TryParseInt32( &integerPart, in ) )
			return false;

		Float32 fractionPart;
		{
			Int32 numerator		= 0;
			Int32 denominator	= 1;

			Int32 dotIndex = FindDot(in);
			if ( dotIndex != -1 )
			{
				in += dotIndex + 1;

				while ( IsNumber( *in ) )
				{
					numerator *= 10;
					numerator += *in - '0';

					denominator *= 10;

					in++;
				}
			}

			fractionPart = static_cast<Float32>(numerator) / denominator;
		}

		if (outputResult)
			*outputResult = integerPart + fractionPart * Sign(integerPart);

		return true;
	}

	Bool TryParseInt32(Int32* outputResult, const Char8* in)
	{
		in = SkipWhitespace(in);

		Int32 sign = 1;
		switch( *in )
		{
		case '-':
			sign = -1;
		case '+':
			in++;
			break;
		}

		if ( !IsNumber( *in ) )
			return false;

		Int32 parse = 0;
		while ( IsNumber( *in ) )
		{
			parse *= 10;
			parse += *in - '0';

			in++;
		}

		if (outputResult)
			*outputResult = parse * sign;

		return true;
	}

	Bool TryConvertBool(Char8* inBuffer, SizeType inSize, Bool value)
	{
		auto Fn = [&](const Char8* text, SizeType size)
			{
				if ( size > inSize )
					return false;

				Memory::Copy(
					text,
					inBuffer,
					sizeof(Char8) * size
				);

				return true;
			};

		if ( !value )
			return Fn( "false", 5 + 1 );

		return Fn( "true", 4 + 1 );
	}

	Bool TryConvertFloat32(Char8* inBuffer, SizeType inSize, Float32 value)
	{
		constexpr RealType Scale		= 100000;
		constexpr SizeType ZeroCount	= 5;


		Int32 sign			= Sign(value);
		Bool isLessThanOne	= false;
		{
			if ( Absolute(value) < 1 )
				isLessThanOne = true;

			if ( sign == 1 && isLessThanOne )
				value += 1;

			if ( sign == -1 && isLessThanOne )
				value -= 1;
		}

		if ( !TryConvertInt32( inBuffer, inSize - 1, value * Scale ) )
			return false;

		SizeType length		= GetUTFElementCount(inBuffer);
		SizeType dotIndex	= length - ZeroCount;
		for (SizeType index = length + 1; index > dotIndex; index--)
			inBuffer[index] = inBuffer[ index - 1 ];

		inBuffer[dotIndex] = '.';

		Int32 replaceIndex = -1;
		{
			if ( sign == 1 && isLessThanOne )
				replaceIndex = 0;

			if ( sign == -1 && isLessThanOne )
				replaceIndex = 1;

			if ( replaceIndex != -1 )
				inBuffer[replaceIndex] = '0';
		}

		return true;
	}

	Bool TryConvertInt32(Char8* inBuffer, SizeType inSize, Int32 value)
	{
		Int32 sign	= Sign(value);
		value		= Absolute(value);

		if ( sign == -1 )
		{
			if ( inSize >= 1 )
			{
				*inBuffer = '-';

				inBuffer++;
				inSize--;
			}
			else
				return false;
		}

		SizeType digits = 1;
		Int32 temporary = value;
		while ( temporary >= 10 )
		{
			temporary /= 10;
			digits++;
		}

		if ( digits + 1 > inSize )
			return false;

		for ( SizeType index = 0; index < digits; index++ )
		{
			Int32 divisor = Power( 10, digits - 1 - index );

			Int32 number	= value / divisor;
			value			-= number * divisor;

			inBuffer[index] = '0' + number;
		}

		inBuffer[digits] = '\0';

		return true;
	}
}