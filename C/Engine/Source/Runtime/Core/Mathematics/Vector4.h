#pragma once

#include "Math.h"



typedef union
{
	struct
	{
		RealType x;
		RealType y;
		RealType z;
		RealType w;
	};
	RealType components[4];
}
Vector4;


struct FVector4
{
	const Vector4 Zero;
	const Vector4 One;


	RealType(*Dot)(const Vector4* leftHandSide, const Vector4* rightHandSide);


	Vector4(*Multiply)(const Vector4* instance, RealType scalar);
	Vector4(*Divide)(const Vector4* instance, RealType scalar);
};


extern ENGINE_API struct FVector4 FVector4;