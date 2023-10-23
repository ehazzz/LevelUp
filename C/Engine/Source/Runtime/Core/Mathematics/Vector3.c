#include "Vector3.h"



static Vector3 Cross(const Vector3* leftHandSide, const Vector3* rightHandSide)
{
	Vector3 ret =
	{
		leftHandSide->y * rightHandSide->z - leftHandSide->z * rightHandSide->y,
		leftHandSide->z * rightHandSide->x - leftHandSide->x * rightHandSide->z,
		leftHandSide->x * rightHandSide->y - leftHandSide->y * rightHandSide->x,
	};

	return ret;
}

static RealType Dot(const Vector3* leftHandSide, const Vector3* rightHandSide)
{
	return
		leftHandSide->x * rightHandSide->x +
		leftHandSide->y * rightHandSide->y +
		leftHandSide->z * rightHandSide->z;
}

static RealType Magnitude(const Vector3* instance)
{
	return FMath.SquareRoot( FVector3.Dot(instance, instance) );
}

static Vector3 Normalize(const Vector3* instance)
{
	return FVector3.Divide( instance, FVector3.Magnitude(instance) );
}

static Vector3 Add(const Vector3* leftHandSide, const Vector3* rightHandSide)
{
	Vector3 ret;
	ret.x = leftHandSide->x + rightHandSide->x;
	ret.y = leftHandSide->y + rightHandSide->y;
	ret.z = leftHandSide->z + rightHandSide->z;

	return ret;
}

static Vector3 Subtract(const Vector3* leftHandSide, const Vector3* rightHandSide)
{
	Vector3 ret;
	ret.x = leftHandSide->x - rightHandSide->x;
	ret.y = leftHandSide->y - rightHandSide->y;
	ret.z = leftHandSide->z - rightHandSide->z;

	return ret;
}

static Vector3 Multiply(const Vector3* instance, RealType scalar)
{
	Vector3 ret;
	ret.x = instance->x * scalar;
	ret.y = instance->y * scalar;
	ret.z = instance->z * scalar;

	return ret;
}

static Vector3 Divide(const Vector3* instance, RealType scalar)
{
	Vector3 ret;
	ret.x = instance->x / scalar;
	ret.y = instance->y / scalar;
	ret.z = instance->z / scalar;

	return ret;
}


struct FVector3 FVector3 =
{
	{  0,  0,  0 },
	{  1,  1,  1 },
	{ -1,  0,  0 },
	{  1,  0,  0 },
	{  0, -1,  0 },
	{  0,  1,  0 },
	{  0,  0, -1 },
	{  0,  0,  1 },

	Cross,
	Dot,
	Magnitude,
	Normalize,
	Add,
	Subtract,
	Multiply,
	Divide,
};
