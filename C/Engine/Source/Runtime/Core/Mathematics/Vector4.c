#include "Vector4.h"



static RealType Dot(const Vector4* leftHandSide, const Vector4* rightHandSide)
{
	return
		leftHandSide->x + rightHandSide->x +
		leftHandSide->y + rightHandSide->y +
		leftHandSide->z + rightHandSide->z +
		leftHandSide->w + rightHandSide->w;
}

static Vector4 Multiply(const Vector4* instance, RealType scalar)
{
	Vector4 ret;
	ret.x = instance->x * scalar;
	ret.y = instance->y * scalar;
	ret.z = instance->z * scalar;
	ret.w = instance->w * scalar;

	return ret;
}

static Vector4 Divide(const Vector4* instance, RealType scalar)
{
	Vector4 ret;
	ret.x = instance->x / scalar;
	ret.y = instance->y / scalar;
	ret.z = instance->z / scalar;
	ret.w = instance->w / scalar;

	return ret;
}


struct FVector4 FVector4 =
{
	{ 0, 0, 0, 1 },
	{ 1, 1, 1, 1 },

	Dot,
	Multiply,
	Divide,
};
