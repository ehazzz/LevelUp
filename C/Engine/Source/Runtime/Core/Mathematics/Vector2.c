#include "Vector2.h"



static RealType Cross(const Vector2* leftHandSide, const Vector2* rightHandSide)
{
	return leftHandSide->x * rightHandSide->y - leftHandSide->y * rightHandSide->x;
}

static RealType Dot(const Vector2* leftHandSide, const Vector2* rightHandSide)
{
	return
		leftHandSide->x * rightHandSide->x +
		leftHandSide->y * rightHandSide->y;
}

static RealType Magnitude(const Vector2* instance)
{
	return FMath.SquareRoot( FVector2.Dot(instance, instance) );
}

static Vector2 Normalize(const Vector2* instance)
{
	return FVector2.Divide( instance, FVector2.Magnitude(instance) );
}

static Vector2 Add(const Vector2* leftHandSide, const Vector2* rightHandSide)
{
	Vector2 ret;
	ret.x = leftHandSide->x + rightHandSide->x;
	ret.y = leftHandSide->y + rightHandSide->y;

	return ret;
}

static Vector2 Subtract(const Vector2* leftHandSide, const Vector2* rightHandSide)
{
	Vector2 ret;
	ret.x = leftHandSide->x - rightHandSide->x;
	ret.y = leftHandSide->y - rightHandSide->y;

	return ret;
}

static Vector2 Multiply(const Vector2* instance, RealType scalar)
{
	Vector2 ret;
	ret.x = instance->x * scalar;
	ret.y = instance->y * scalar;

	return ret;
}

static Vector2 Divide(const Vector2* instance, RealType scalar)
{
	Vector2 ret;
	ret.x = instance->x / scalar;
	ret.y = instance->y / scalar;

	return ret;
}


struct FVector2 FVector2 =
{
	{  0,  0 },
	{  1,  1 },
	{ -1,  0 },
	{  1,  0 },
	{  0, -1 },
	{  0,  1 },

	Cross,
	Dot,
	Magnitude,
	Normalize,
	Add,
	Subtract,
	Multiply,
	Divide,
};