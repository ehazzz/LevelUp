#include "Vector.h"



namespace Engine
{
	const Vector2 Vector2_Zero		= {  0,  0 };
	const Vector2 Vector2_One		= {  1,  1 };
	const Vector2 Vector2_Left		= { -1,  0 };
	const Vector2 Vector2_Right		= {  1,  0 };
	const Vector2 Vector2_Down		= {  0, -1 };
	const Vector2 Vector2_Up		= {  0,  1 };

	const Vector3 Vector3_Zero		= {  0,  0,  0 };
	const Vector3 Vector3_One		= {  1,  1,  1 };
	const Vector3 Vector3_Left		= { -1,  0,  0 };
	const Vector3 Vector3_Right		= {  1,  0,  0 };
	const Vector3 Vector3_Down		= {  0, -1,  0 };
	const Vector3 Vector3_Up		= {  0,  1,  0 };
	const Vector3 Vector3_Back		= {  0,  0, -1 };
	const Vector3 Vector3_Forward	= {  0,  0,  1 };

	const Vector4 Vector4_Zero		= { 0, 0, 0, 1 };
	const Vector4 Vector4_One		= { 1, 1, 1, 1 };


	const Vector2Int Vector2Int_Zero	= {  0,  0 };
	const Vector2Int Vector2Int_One		= {  1,  1 };
	const Vector2Int Vector2Int_Left	= { -1,  0 };
	const Vector2Int Vector2Int_Right	= {  1,  0 };
	const Vector2Int Vector2Int_Down	= {  0, -1 };
	const Vector2Int Vector2Int_Up		= {  0,  1 };

	const Vector3Int Vector3Int_Zero	= {  0,  0,  0 };
	const Vector3Int Vector3Int_One		= {  1,  1,  1 };
	const Vector3Int Vector3Int_Left	= { -1,  0,  0 };
	const Vector3Int Vector3Int_Right	= {  1,  0,  0 };
	const Vector3Int Vector3Int_Down	= {  0, -1,  0 };
	const Vector3Int Vector3Int_Up		= {  0,  1,  0 };
	const Vector3Int Vector3Int_Back	= {  0,  0, -1 };
	const Vector3Int Vector3Int_Forward	= {  0,  0,  1 };

	const Vector4Int Vector4Int_Zero	= { 0, 0, 0, 1 };
	const Vector4Int Vector4Int_One		= { 1, 1, 1, 1 };


	Vector3 RodriguesRotationFormula(const Vector3& rotationAxis, RealType radians, const Vector3& point)
	{
		/**
		* u		= rotationAxis
		* theta	= radians
		* v		= point
		*
		* v' = Cosine(theta) * v + ( 1 - Cosine(theta) ) * Dot(u, v) * u + Sine(theta) * Cross(u, v)
		*/

		Vector3 u		= Normalize(rotationAxis);
		RealType theta	= radians;
		Vector3 v		= point;

		RealType sine	= Sine(theta);
		RealType cosine	= Cosine(theta);

		return cosine * v + ( 1 - cosine ) * Dot(u, v) * u + sine * Cross(u, v);
	}
}