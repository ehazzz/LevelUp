#include "Matrix3.h"



static Void SetRow(Matrix3* instance, SizeType index, const Vector3* vector3)
{
	instance->rows[index] = *vector3;
}

static Void SetColumn(Matrix3* instance, SizeType index, const Vector3* vector3)
{
	for ( SizeType iteration = 0; iteration < 3; iteration++ )
		instance->components[ 3 * iteration + index ] = vector3->components[iteration];
}

static Vector3 GetRow(const Matrix3* instance, SizeType index)
{
	return instance->rows[index];
}

static Vector3 GetColumn(const Matrix3* instance, SizeType index)
{
	Vector3 ret;
	for ( SizeType iteration = 0; iteration < 3; iteration++ )
		ret.components[iteration] = instance->components[ 3 * iteration + index ];

	return ret;
}

static Matrix3 Transpose(const Matrix3* instance)
{
	Matrix3 ret;
	for ( SizeType y = 0; y < 3; y++ )
		for ( SizeType x = 0; x < 3; x++ )
			ret.components[ 3 * x + y ] = instance->components[ 3 * y + x ];

	return ret;
}

static Matrix3 Multiply(const Matrix3* leftHandSide, const Matrix3* rightHandSide)
{
	Matrix3 ret;
	for ( SizeType y = 0; y < 3; y++ )
	{
		Vector3 column = FMatrix3.GetColumn(rightHandSide, y);

		for ( SizeType x = 0; x < 3; x++ )
			ret.components[ 3 * x + y ] = FVector3.Dot( leftHandSide->rows + x, &column );
	}

	return ret;
}

static Vector3 MultiplyMV(const Matrix3* instance, const Vector3* vector3)
{
	Vector3 ret;
	for ( SizeType index = 0; index < 3; index++ )
		ret.components[index] = FVector3.Dot( instance->rows + index, vector3 );

	return ret;
}


struct FMatrix3 FMatrix3 = 
{
	{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
	},

	SetRow,
	SetColumn,
	GetRow,
	GetColumn,
	Transpose,
	Multiply,
	MultiplyMV,
};
