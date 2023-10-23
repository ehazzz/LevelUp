#include "Color.h"



namespace Engine
{
	const Color Color_Black		= { 0, 0, 0, 1 };
	const Color Color_White		= { 1, 1, 1, 1 };
	const Color Color_Red		= { 1, 0, 0, 1 };
	const Color Color_Green		= { 0, 1, 0, 1 };
	const Color Color_Blue		= { 0, 0, 1, 1 };
	const Color Color_Yellow	= { 1, 1, 0, 1 };
	const Color Color_Cyan		= { 0, 1, 1, 1 };
	const Color Color_Magenta	= { 1, 0, 1, 1 };


	RealType Luminance(const Color& instance)
	{
		return
			instance[0] * 0.2126 +
			instance[1] * 0.7152 +
			instance[2] * 0.0722;
	}

	Color Darkened(const Color& instance, RealType amount)
	{
		amount = 1 - amount;

		return Color(
			instance[0] * amount,
			instance[1] * amount,
			instance[2] * amount,
			instance[3]
		);
	}

	Color Lightened(const Color& instance, RealType amount)
	{
		return Color(
			instance[0] + ( 1 - instance[0] ) * amount,
			instance[1] + ( 1 - instance[1] ) * amount,
			instance[2] + ( 1 - instance[2] ) * amount,
			instance[3]
		);
	}

	Color ToLinear(const Color& instance)
	{
		constexpr RealType Exponent = 2.2;


		return Color(
			Power( instance[0], Exponent ),
			Power( instance[1], Exponent ),
			Power( instance[2], Exponent ),
			instance[3]
		);
	}

	Color ToSrgb(const Color& instance)
	{
		constexpr RealType Exponent = 1 / 2.2;


		return Color(
			Power( instance[0], Exponent ),
			Power( instance[1], Exponent ),
			Power( instance[2], Exponent ),
			instance[3]
		);
	}
}