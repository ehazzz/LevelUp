#pragma once

#include "Vector2.h"
#include "Vector3.h"



struct FBezierCurve
{
	RealType(*Evaluate1D)(RealType* controlPoints, SizeType count, RealType time);
	Vector2(*Evaluate2D)(Vector2* controlPoints, SizeType count, RealType time);
	Vector3(*Evaluate3D)(Vector3* controlPoints, SizeType count, RealType time);
};


extern ENGINE_API struct FBezierCurve FBezierCurve;