#include "BezierCurve.h"



static Vector2 Lerp2D(const Vector2* from, const Vector2* to, RealType amount)
{
	Vector2 ret;
	ret.x = FMath.Lerp( from->x, to->x, amount );
	ret.y = FMath.Lerp( from->y, to->y, amount );

	return ret;
}

static Vector3 Lerp3D(const Vector3* from, const Vector3* to, RealType amount)
{
	Vector3 ret;
	ret.x = FMath.Lerp( from->x, to->x, amount );
	ret.y = FMath.Lerp( from->y, to->y, amount );
	ret.z = FMath.Lerp( from->z, to->z, amount );

	return ret;
}


static RealType Evaluate1D(RealType* controlPoints, SizeType count, RealType time)
{
	if ( count == 1 )
		return controlPoints[0];

	for (SizeType index = 1; index < count; index++)
	{
		SizeType begin	= index - 1;
		SizeType end	= index;

		controlPoints[begin] = FMath.Lerp(
			controlPoints[begin],
			controlPoints[end],
			time
		);
	}

	return FBezierCurve.Evaluate1D( controlPoints, count - 1, time );
}

static Vector2 Evaluate2D(Vector2* controlPoints, SizeType count, RealType time)
{
	if ( count == 1 )
		return controlPoints[0];

	for (SizeType index = 1; index < count; index++)
	{
		SizeType begin	= index - 1;
		SizeType end	= index;

		controlPoints[begin] = Lerp2D(
			&controlPoints[begin],
			&controlPoints[end],
			time
		);
	}

	return FBezierCurve.Evaluate2D( controlPoints, count - 1, time );
}

static Vector3 Evaluate3D(Vector3* controlPoints, SizeType count, RealType time)
{
	if ( count == 1 )
		return controlPoints[0];

	for (SizeType index = 1; index < count; index++)
	{
		SizeType begin	= index - 1;
		SizeType end	= index;

		controlPoints[begin] = Lerp3D(
			&controlPoints[begin],
			&controlPoints[end],
			time
		);
	}

	return FBezierCurve.Evaluate3D( controlPoints, count - 1, time );
}


struct FBezierCurve FBezierCurve =
{
	Evaluate1D,
	Evaluate2D,
	Evaluate3D,
};