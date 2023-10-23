#pragma once

#include "Math.h"



typedef union
{
	struct
	{
		RealType x;
		RealType y;
		union
		{
			RealType z;
			RealType w; // Usages only for 2D homogeneous coordinates representation
		};
	};
	RealType components[3];
}
Vector3;


struct FVector3
{
	const Vector3 Zero;
	const Vector3 One;
	const Vector3 Left;
	const Vector3 Right;
	const Vector3 Down;
	const Vector3 Up;
	const Vector3 Back;
	const Vector3 Forward;


	Vector3(*Cross)(const Vector3* leftHandSide, const Vector3* rightHandSide);
	RealType(*Dot)(const Vector3* leftHandSide, const Vector3* rightHandSide);
	RealType(*Magnitude)(const Vector3* instance);
	Vector3(*Normalize)(const Vector3* instance);


	Vector3(*Add)(const Vector3* leftHandSide, const Vector3* rightHandSide);
	Vector3(*Subtract)(const Vector3* leftHandSide, const Vector3* rightHandSide);
	Vector3(*Multiply)(const Vector3* instance, RealType scalar);
	Vector3(*Divide)(const Vector3* instance, RealType scalar);
};


extern ENGINE_API struct FVector3 FVector3;