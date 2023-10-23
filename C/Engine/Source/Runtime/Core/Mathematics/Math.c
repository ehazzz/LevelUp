#include "Math.h"

#include <math.h>



static RealType Absolute(RealType value)
{
	return value < 0 ? -value : value;
}

static RealType Clamp(RealType value, RealType minimum, RealType maximum)
{
	return value > maximum ? maximum :
		value < minimum ? minimum : value;
}

static RealType Clamp01(RealType value)
{
	return FMath.Clamp(value, 0, 1);
}

static RealType Lerp(RealType from, RealType to, RealType amount)
{
	return from + ( to - from ) * amount;
}

static RealType Maximum(RealType a, RealType b)
{
	return a > b ? a : b;
}

static RealType Minimum(RealType a, RealType b)
{
	return a < b ? a : b;
}

static RealType Power(RealType base, RealType exponent)
{
	return powf(base, exponent);
}

static RealType SquareRoot(RealType value)
{
	return sqrtf(value);
}

static RealType Sine(RealType radians)
{
	return sinf(radians);
}

static RealType Cosine(RealType radians)
{
	return cosf(radians);
}

static RealType Tangent(RealType radians)
{
	return tanf(radians);
}


struct FMath FMath =
{
	3.14159265358979323846,
	57.29577951308232,
	0.017453292519943295,

	Absolute,
	Clamp,
	Clamp01,
	Lerp,
	Maximum,
	Minimum,
	Power,
	SquareRoot,
	Sine,
	Cosine,
	Tangent,
};