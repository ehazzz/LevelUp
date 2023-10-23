#include "Math.h"

#include <cmath>



namespace Engine
{
	Bool Appropriate(RealType a, RealType b, RealType error)
	{
		return Absolute( a - b ) < error;
	}

	RealType Ceil(RealType value)
	{
		return static_cast<Int32>( value + 0.5 );
	}

	RealType Clamp01(RealType value)
	{
		return Clamp<RealType>(value, 0, 1);
	}

	RealType Floor(RealType value)
	{
		return static_cast<Int32>(value);
	}

	/*
	RealType Power(RealType value, SizeType exponent)
	{
		return exponent == 0 ? 1 : value * Power( value, exponent - 1 );
	}
	*/

	RealType Power(RealType value, RealType exponent)
	{
		return std::pow(value, exponent);
	}

	RealType SquareRoot(RealType value)
	{
		return std::sqrt(value);
	}

	RealType Sine(RealType radians)
	{
		return std::sin(radians);
	}

	RealType Cosine(RealType radians)
	{
		return std::cos(radians);
	}

	RealType Tangent(RealType radians)
	{
		return std::tan(radians);
	}
}