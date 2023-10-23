#pragma once

#include "Math.h"

#include "../System/Memory.h"



namespace Engine
{
	using Dimension = SizeType;


	template<Dimension, typename T> struct Vector;

	using Vector2 = Vector<2, RealType>;
	using Vector3 = Vector<3, RealType>;
	using Vector4 = Vector<4, RealType>;

	using Vector2Int = Vector<2, Int32>;
	using Vector3Int = Vector<3, Int32>;
	using Vector4Int = Vector<4, Int32>;


	extern ENGINE_API const Vector2 Vector2_Zero;
	extern ENGINE_API const Vector2 Vector2_One;
	extern ENGINE_API const Vector2 Vector2_Left;
	extern ENGINE_API const Vector2 Vector2_Right;
	extern ENGINE_API const Vector2 Vector2_Down;
	extern ENGINE_API const Vector2 Vector2_Up;

	extern ENGINE_API const Vector3 Vector3_Zero;
	extern ENGINE_API const Vector3 Vector3_One;
	extern ENGINE_API const Vector3 Vector3_Left;
	extern ENGINE_API const Vector3 Vector3_Right;
	extern ENGINE_API const Vector3 Vector3_Down;
	extern ENGINE_API const Vector3 Vector3_Up;
	extern ENGINE_API const Vector3 Vector3_Back;
	extern ENGINE_API const Vector3 Vector3_Forward;

	extern ENGINE_API const Vector4 Vector4_Zero;
	extern ENGINE_API const Vector4 Vector4_One;


	extern ENGINE_API const Vector2Int Vector2Int_Zero;
	extern ENGINE_API const Vector2Int Vector2Int_One;
	extern ENGINE_API const Vector2Int Vector2Int_Left;
	extern ENGINE_API const Vector2Int Vector2Int_Right;
	extern ENGINE_API const Vector2Int Vector2Int_Down;
	extern ENGINE_API const Vector2Int Vector2Int_Up;

	extern ENGINE_API const Vector3Int Vector3Int_Zero;
	extern ENGINE_API const Vector3Int Vector3Int_One;
	extern ENGINE_API const Vector3Int Vector3Int_Left;
	extern ENGINE_API const Vector3Int Vector3Int_Right;
	extern ENGINE_API const Vector3Int Vector3Int_Down;
	extern ENGINE_API const Vector3Int Vector3Int_Up;
	extern ENGINE_API const Vector3Int Vector3Int_Back;
	extern ENGINE_API const Vector3Int Vector3Int_Forward;

	extern ENGINE_API const Vector4Int Vector4Int_Zero;
	extern ENGINE_API const Vector4Int Vector4Int_One;


	// Vector representation
	ENGINE_API Vector3 RodriguesRotationFormula(const Vector3& rotationAxis, RealType radians, const Vector3& point);


	template<typename T> Vector<2, T> ToVector2(const Vector<3, T>& instance);
	template<typename T> Vector<2, T> ToVector2(const Vector<4, T>& instance);
	template<typename T> Vector<3, T> ToVector3(const Vector<2, T>& instance, T z = T());
	template<typename T> Vector<3, T> ToVector3(const Vector<4, T>& instance);
	template<typename T> Vector<4, T> ToVector4(const Vector<2, T>& instance, T z = T(), T w = T());
	template<typename T> Vector<4, T> ToVector4(const Vector<3, T>& instance, T w = T());

	template<Dimension D, typename T> Vector<D, T> Lerp(const Vector<D, T>& leftHandSide, const Vector<D, T>& rightHandSide, RealType amount);

	template<Dimension D, typename T> T Dot(const Vector<D, T>& leftHandSide, const Vector<D, T>& rightHandSide);
	template<Dimension D, typename T> auto Cross(const Vector<D, T>& leftHandSide, const Vector<D, T>& rightHandSide);
	template<Dimension D, typename T> T Magnitude(const Vector<D, T>& instance);
	template<Dimension D, typename T> Vector<D, T> Normalize(const Vector<D, T>& instance);


	template<Dimension D, typename T>
	struct Vector
	{
		T components[D];


		Vector();
		template<typename ...Parameters> Vector(Parameters ...parameters);
		Vector(const Vector<D, T>& other);


		Vector<D, T>& operator=(const Vector<D, T>& other);

		T& operator[](SizeType index);
		const T& operator[](SizeType index) const;

		Vector<D, T> operator+() const;
		Vector<D, T> operator-() const;

		Vector<D, T>& operator++();
		Vector<D, T> operator++(Int32);

		Vector<D, T>& operator--();
		Vector<D, T> operator--(Int32);

		Vector<D, T> operator+(const Vector<D, T>& other) const;
		Vector<D, T> operator-(const Vector<D, T>& other) const;
		Vector<D, T> operator*(const Vector<D, T>& other) const;

		Vector<D, T> operator*(T scalar) const;
		Vector<D, T> operator/(T scalar) const;
		friend inline Vector<D, T> operator*(T scalar, const Vector<D, T>& instance) { return instance * scalar; }

		Vector<D, T>& operator+=(const Vector<D, T>& other);
		Vector<D, T>& operator-=(const Vector<D, T>& other);
		Vector<D, T>& operator*=(const Vector<D, T>& other);

		Vector<D, T>& operator*=(T scalar);
		Vector<D, T>& operator/=(T scalar);
	};


#pragma region Details
	template<typename T>
	Vector<2, T> ToVector2(const Vector<3, T>& instance)
	{
		return Vector<2, T>( instance[0], instance[1] );
	}

	template<typename T>
	Vector<2, T> ToVector2(const Vector<4, T>& instance)
	{
		return Vector<2, T>( instance[0], instance[1] );
	}

	template<typename T>
	Vector<3, T> ToVector3(const Vector<2, T>& instance, T z)
	{
		return Vector<3, T>( instance[0], instance[1], z );
	}

	template<typename T>
	Vector<3, T> ToVector3(const Vector<4, T>& instance)
	{
		return Vector<3, T>( instance[0], instance[1], instance[2] );
	}

	template<typename T>
	Vector<4, T> ToVector4(const Vector<2, T>& instance, T z, T w)
	{
		return Vector<4, T>( instance[0], instance[1], z, w );
	}

	template<typename T>
	Vector<4, T> ToVector4(const Vector<3, T>& instance, T w)
	{
		return Vector<4, T>( instance[0], instance[1], instance[2], w );
	}

	template<Dimension D, typename T>
	Vector<D, T> Lerp(const Vector<D, T>& leftHandSide, const Vector<D, T>& rightHandSide, RealType amount)
	{
		Vector<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = Lerp( leftHandSide[index], rightHandSide[index], amount );

		return ret;
	}

	template<Dimension D, typename T>
	T Dot(const Vector<D, T>& leftHandSide, const Vector<D, T>& rightHandSide)
	{
		T ret = T();
		for ( SizeType index = 0; index < D; index++ )
			ret += leftHandSide[index] * rightHandSide[index];

		return ret;
	}

	template<Dimension D, typename T>
	auto Cross(const Vector<D, T>& leftHandSide, const Vector<D, T>& rightHandSide)
	{
		static_assert( D == 2 || D == 3 );


		if constexpr ( D == 2 )
			return leftHandSide[0] * rightHandSide[1] - leftHandSide[1] * rightHandSide[0]; // xy - yx

		if constexpr ( D == 3 )
			return Vector<3, T>(
				leftHandSide[1] * rightHandSide[2] - leftHandSide[2] * rightHandSide[1], // yz - zy
				leftHandSide[2] * rightHandSide[0] - leftHandSide[0] * rightHandSide[2], // zx - xz
				leftHandSide[0] * rightHandSide[1] - leftHandSide[1] * rightHandSide[0]  // xy - yx
			);
	}

	template<Dimension D, typename T>
	T Magnitude(const Vector<D, T>& instance)
	{
		return SquareRoot( Dot(instance, instance) );
	}

	template<Dimension D, typename T>
	Vector<D, T> Normalize(const Vector<D, T>& instance)
	{
		return instance / Magnitude(instance);
	}

	template<Dimension D, typename T>
	Vector<D, T>::Vector()
	{

	}

	template<Dimension D, typename T>
	template<typename ...Parameters>
	Vector<D, T>::Vector(Parameters ...parameters)
	{
		static_assert( sizeof...(parameters) <= D );


		ParameterPackHelper::CopyTo( components, parameters... );
	}

	template<Dimension D, typename T>
	Vector<D, T>::Vector(const Vector<D, T>& other)
	{
		Memory::Copy(
			other.components,
			components,
			sizeof(T) * D
		);
	}

	template<Dimension D, typename T>
	Vector<D, T>& Vector<D, T>::operator=(const Vector<D, T>& other)
	{
		Memory::Copy(
			other.components,
			components,
			sizeof(T) * D
		);

		return *this;
	}

	template<Dimension D, typename T>
	T& Vector<D, T>::operator[](SizeType index)
	{
		return components[index];
	}

	template<Dimension D, typename T>
	const T& Vector<D, T>::operator[](SizeType index) const
	{
		return components[index];
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator+() const
	{
		return *this;
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator-() const
	{
		Vector<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = -components[index];

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T>& Vector<D, T>::operator++()
	{
		for ( SizeType index = 0; index < D; index++ )
			components[index] += 1;

		return *this;
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator++(Int32)
	{
		auto ret = *this;

		++( *this );

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T>& Vector<D, T>::operator--()
	{
		for ( SizeType index = 0; index < D; index++ )
			components[index] -= 1;

		return *this;
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator--(Int32)
	{
		auto ret = *this;

		--( *this );

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator+(const Vector<D, T>& other) const
	{
		Vector<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = components[index] + other[index];

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator-(const Vector<D, T>& other) const
	{
		Vector<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = components[index] - other[index];

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator*(const Vector<D, T>& other) const
	{
		Vector<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = components[index] * other[index];

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator*(T scalar) const
	{
		Vector<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = components[index] * scalar;

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T> Vector<D, T>::operator/(T scalar) const
	{
		Vector<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = components[index] / scalar;

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T>& Vector<D, T>::operator+=(const Vector<D, T>& other)
	{
		for ( SizeType index = 0; index < D; index++ )
			components[index] += other[index];

		return *this;
	}

	template<Dimension D, typename T>
	Vector<D, T>& Vector<D, T>::operator-=(const Vector<D, T>& other)
	{
		for ( SizeType index = 0; index < D; index++ )
			components[index] -= other[index];

		return *this;
	}

	template<Dimension D, typename T>
	Vector<D, T>& Vector<D, T>::operator*=(const Vector<D, T>& other)
	{
		for ( SizeType index = 0; index < D; index++ )
			components[index] *= other[index];

		return *this;
	}

	template<Dimension D, typename T>
	Vector<D, T>& Vector<D, T>::operator*=(T scalar)
	{
		for ( SizeType index = 0; index < D; index++ )
			components[index] *= scalar;

		return *this;
	}

	template<Dimension D, typename T>
	Vector<D, T>& Vector<D, T>::operator/=(T scalar)
	{
		for ( SizeType index = 0; index < D; index++ )
			components[index] /= scalar;

		return *this;
	}
#pragma endregion
}