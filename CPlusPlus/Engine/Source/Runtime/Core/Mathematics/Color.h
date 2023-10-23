#pragma once

#include "Vector.h"



namespace Engine
{
	using Color = Vector4;


	extern ENGINE_API const Color Color_Black;
	extern ENGINE_API const Color Color_White;
	extern ENGINE_API const Color Color_Red;
	extern ENGINE_API const Color Color_Green;
	extern ENGINE_API const Color Color_Blue;
	extern ENGINE_API const Color Color_Yellow;
	extern ENGINE_API const Color Color_Cyan;
	extern ENGINE_API const Color Color_Magenta;


	RealType Luminance(const Color& instance);

	Color Darkened(const Color& instance, RealType amount);
	Color Lightened(const Color& instance, RealType amount);

	/**
	* Convert to linear color space
	* 
	* 
	* @remark
	* Gamma 0.45 to Gamma 1.0
	*/
	Color ToLinear(const Color& instance);
	/**
	* Convert to gamma color space
	* 
	* 
	* @remark
	* Gamma 1.0 to Gamma 0.45
	*/
	Color ToSrgb(const Color& instance);
}