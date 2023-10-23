#include "Matrix3Extension.h"



static Matrix3 Rotate(RealType radians)
{
	RealType sine	= FMath.Sine(radians);
	RealType cosine = FMath.Cosine(radians);

	Matrix3 ret = FMatrix3.Identity;
	ret.m00		= cosine;
	ret.m10		= sine;
	ret.m01		= -sine;
	ret.m11		= cosine;

	return ret;
}

static Matrix3 Scale(RealType scaleX, RealType scaleY)
{
	Matrix3 ret	= FMatrix3.Identity;
	ret.m00		= scaleX;
	ret.m11		= scaleY;

	return ret;
}

static Matrix3 Translate(RealType translationX, RealType translationY)
{
	Matrix3 ret	= FMatrix3.Identity;
	ret.m02		= translationX;
	ret.m12		= translationY;

	return ret;
}


struct FMatrix3Extension FMatrix3Extension =
{
	Rotate,
	Scale,
	Translate,
};