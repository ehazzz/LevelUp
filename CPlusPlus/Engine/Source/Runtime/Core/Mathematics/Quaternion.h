#pragma once

#include "Vector.h"



namespace Engine
{
	struct ENGINE_API Quaternion
	{
		union
		{
			RealType scalar;
			RealType w;
		};


		union
		{
			Vector3 vector;
			struct
			{
				RealType x;
				RealType y;
				RealType z;
			};
		};


		Quaternion();
		Quaternion(RealType w, RealType x, RealType y, RealType z);
		Quaternion(const Vector3& vector3);
		Quaternion(const Quaternion& other);


		Quaternion Conjugate() const;
		Quaternion Inverse() const;
		RealType Magnitude() const;


		Quaternion& operator=(const Quaternion& other);

		RealType& operator[](SizeType index);
		const RealType& operator[](SizeType index) const;

		Quaternion operator+(const Quaternion& other) const;
		Quaternion operator-(const Quaternion& other) const;
		Quaternion operator*(const Quaternion& other) const;

		Quaternion operator*(RealType scalar) const;
		friend inline Quaternion operator*(RealType scalar, const Quaternion& instance) { return instance * scalar; }
		Quaternion operator/(RealType scalar) const;

		Quaternion& operator+=(const Quaternion& other);
		Quaternion& operator-=(const Quaternion& other);
		Quaternion& operator*=(const Quaternion& other);

		Quaternion& operator*=(RealType scalar);
		Quaternion& operator/=(RealType scalar);
	};
}