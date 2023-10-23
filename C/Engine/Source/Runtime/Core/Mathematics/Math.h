#pragma once

#include "../Base.h"



struct FMath
{
	const RealType Pi;			// The well-known 3.14159..
	const RealType ToDegrees;	// Radians to degrees conversion constant
	const RealType ToRadians;	// Degrees to radians conversion constant


	RealType(*Absolute)(RealType value);
	RealType(*Clamp)(RealType value, RealType minimum, RealType maximum);
	RealType(*Clamp01)(RealType value);
	RealType(*Lerp)(RealType from, RealType to, RealType amount);
	RealType(*Maximum)(RealType a, RealType b);
	RealType(*Minimum)(RealType a, RealType b);
	RealType(*Power)(RealType base, RealType exponent);
	RealType(*SquareRoot)(RealType value);

	RealType(*Sine)(RealType radians);
	RealType(*Cosine)(RealType radians);
	RealType(*Tangent)(RealType radians);
};


extern ENGINE_API struct FMath FMath;