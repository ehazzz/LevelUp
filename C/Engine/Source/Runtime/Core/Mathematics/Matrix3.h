#pragma once

#include "Vector3.h"



// Row major matrix, and apply to a vector should use left-multiplication
typedef union
{
	struct
	{
		RealType m00, m01, m02;
		RealType m10, m11, m12;
		RealType m20, m21, m22;
	};
	RealType	components[ 3 * 3 ];
	Vector3		rows[3];
}
Matrix3;


struct FMatrix3
{
	const Matrix3 Identity;


	Void(*SetRow)(Matrix3* instance, SizeType index, const Vector3* vector3);
	Void(*SetColumn)(Matrix3* instance, SizeType index, const Vector3* vector3);
	Vector3(*GetRow)(const Matrix3* instance, SizeType index);
	Vector3(*GetColumn)(const Matrix3* instance, SizeType index);

	Matrix3(*Transpose)(const Matrix3* instance);

	Matrix3(*Multiply)(const Matrix3* leftHandSide, const Matrix3* rightHandSide);
	Vector3(*MultiplyMV)(const Matrix3* instance, const Vector3* vector3);
};


extern ENGINE_API struct FMatrix3 FMatrix3;
