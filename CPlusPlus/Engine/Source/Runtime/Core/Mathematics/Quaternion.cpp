#include "Quaternion.h"



namespace Engine
{
	Quaternion::Quaternion()
	{

	}

	Quaternion::Quaternion(RealType w, RealType x, RealType y, RealType z)
		: w(w), x(x), y(y), z(z)
	{

	}

	Quaternion::Quaternion(const Vector3& vector3)
		: scalar(0), vector(vector3)
	{

	}

	Quaternion::Quaternion(const Quaternion& other)
	{
		Memory::Copy(
			&other,
			this,
			sizeof(Quaternion)
		);
	}

	Quaternion Quaternion::Conjugate() const
	{
		Quaternion ret;
		ret.scalar = scalar;
		ret.vector = -vector;

		return ret;
	}

	Quaternion Quaternion::Inverse() const
	{
		return Conjugate() / ( scalar * scalar + Dot(vector, vector) );
	}

	RealType Quaternion::Magnitude() const
	{
		return SquareRoot( scalar * scalar + Dot(vector, vector) );
	}

	Quaternion& Quaternion::operator=(const Quaternion& other)
	{
		Memory::Copy(
			&other,
			this,
			sizeof(Quaternion)
		);

		return *this;
	}

	RealType& Quaternion::operator[](SizeType index)
	{
		return reinterpret_cast<RealType*>(this)[index];
	}

	const RealType& Quaternion::operator[](SizeType index) const
	{
		return reinterpret_cast<const RealType*>(this)[index];
	}

	Quaternion Quaternion::operator+(const Quaternion& other) const
	{
		Quaternion ret;
		ret.scalar = scalar + other.scalar;
		ret.vector = vector + other.vector;

		return ret;
	}

	Quaternion Quaternion::operator-(const Quaternion& other) const
	{
		Quaternion ret;
		ret.scalar = scalar - other.scalar;
		ret.vector = vector - other.vector;

		return ret;
	}

	Quaternion Quaternion::operator*(const Quaternion& other) const
	{
		/**
		* q1 = [s1, v1]
		* q2 = [s2, v2]
		* 
		* q1 * q2 = [ s1 * s1 - Dot(v1, v2), s1 * v2 + s2 * v1 + Cross(v1, v2) ]
		*/

		Quaternion ret;
		ret.scalar = scalar * other.scalar - Dot( vector, other.vector );
		ret.vector = 
			scalar			* other.vector	+ 
			other.scalar	* vector		+
			Cross( vector, other.vector );

		return ret;
	}

	Quaternion Quaternion::operator*(RealType scalar) const
	{
		Quaternion ret;
		ret.scalar = this->scalar * scalar;
		ret.vector = this->vector * scalar;

		return ret;
	}

	Quaternion Quaternion::operator/(RealType scalar) const
	{
		Quaternion ret;
		ret.scalar = this->scalar / scalar;
		ret.vector = this->vector / scalar;

		return ret;
	}

	Quaternion& Quaternion::operator+=(const Quaternion& other)
	{
		*this = *this + other;

		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& other)
	{
		*this = *this - other;

		return *this;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& other)
	{
		*this = *this * other;

		return *this;
	}

	Quaternion& Quaternion::operator*=(RealType scalar)
	{
		*this = *this * scalar;

		return *this;
	}

	Quaternion& Quaternion::operator/=(RealType scalar)
	{
		*this = *this / scalar;

		return *this;
	}
}