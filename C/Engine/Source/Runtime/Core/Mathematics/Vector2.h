#pragma once

#include "Math.h"



typedef union
{
	struct 
	{
		RealType x;
		RealType y;
	};
	RealType components[2];
}
Vector2;


struct FVector2
{
	const Vector2 Zero;
	const Vector2 One;
	const Vector2 Left;
	const Vector2 Right;
	const Vector2 Down;
	const Vector2 Up;


	RealType(*Cross)(const Vector2* leftHandSide, const Vector2* rightHandSide);
	RealType(*Dot)(const Vector2* leftHandSide, const Vector2* rightHandSide);
	RealType(*Magnitude)(const Vector2* instance);
	Vector2(*Normalize)(const Vector2* instance);


	Vector2(*Add)(const Vector2* leftHandSide, const Vector2* rightHandSide);
	Vector2(*Subtract)(const Vector2* leftHandSide, const Vector2* rightHandSide);
	Vector2(*Multiply)(const Vector2* instance, RealType scalar);
	Vector2(*Divide)(const Vector2* instance, RealType scalar);
};


extern ENGINE_API struct FVector2 FVector2;