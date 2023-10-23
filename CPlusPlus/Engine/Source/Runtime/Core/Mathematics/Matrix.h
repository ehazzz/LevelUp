#pragma once

#include "Vector.h"



namespace Engine
{
	// Row major matrix, and apply to a vector should use left-multiplication
	template<Dimension, typename T> struct Matrix;

	using Matrix2 = Matrix<2, RealType>;
	using Matrix3 = Matrix<3, RealType>;
	using Matrix4 = Matrix<4, RealType>;


	extern ENGINE_API const Matrix2 Matrix2_Identity;
	extern ENGINE_API const Matrix3 Matrix3_Identity;
	extern ENGINE_API const Matrix4 Matrix4_Identity;


	template<Dimension D, typename T> T Determinant(const Matrix<D, T>& instance);
	/**
	* @remark
	* A-1: matrix's inverse
	* |A|: matrix's determinant
	* A* : adjoint matrix
	* 
	* 
	* A-1 = 1 / |A| * A*
	*/
	template<Dimension D, typename T> Matrix<D, T> Inverse(const Matrix<D, T>& instance);
	template<Dimension D, typename T> Matrix<D, T> Transpose(const Matrix<D, T>& instance);


	/**
	* @remark
	* 2D transformations
	*/
	ENGINE_API Matrix3 Rotate(RealType radians);
	ENGINE_API Matrix3 Scale(const Vector2& scale);
	ENGINE_API Matrix3 Translate(const Vector2& translation);


	/**
	* @remark
	* 3D transformations
	*/
	ENGINE_API Matrix4 RotateX(RealType radians);
	ENGINE_API Matrix4 RotateY(RealType radians);
	ENGINE_API Matrix4 RotateZ(RealType radians);
	ENGINE_API Matrix4 Scale(const Vector3& scale);
	ENGINE_API Matrix4 Translate(const Vector3& translation);

	ENGINE_API Matrix4 LookAt(const Vector3& eye, const Vector3& forward, const Vector3& up);

	ENGINE_API Matrix4 OrthographicProjection(RealType left, RealType right, RealType bottom, RealType top, RealType nearZ, RealType farZ);
	ENGINE_API Matrix4 Frustum(RealType left, RealType right, RealType bottom, RealType top, RealType nearZ, RealType farZ);
	ENGINE_API Matrix4 PerspectiveProjection(RealType fovy, RealType aspect, RealType nearZ, RealType farZ);

	ENGINE_API Matrix4 HomogeneousClipSpaceToScreenSpace(RealType width, RealType height);


	template<Dimension D, typename T>
	struct Matrix
	{
		union
		{
			T				components[ D * D ];
			Vector<D, T>	rows[D];
		};


		Matrix();
		template<typename ...Parameters> Matrix(Parameters ...parameters);
		Matrix(const Matrix<D, T>& other);


		Void SetRow(SizeType index, const Vector<D, T>& vector);
		Void SetColumn(SizeType index, const Vector<D, T>& vector);
		const Vector<D, T>& GetRow(SizeType index) const;
		Vector<D, T> GetColumn(SizeType index) const;


		Matrix<D, T>& operator=(const Matrix<D, T>& other);

		Vector<D, T>& operator[](SizeType index);
		const Vector<D, T>& operator[](SizeType index) const;

		Matrix<D, T> operator*(const Matrix<D, T>& other) const;
		Vector<D, T> operator*(const Vector<D, T>& vector) const;

		Matrix<D, T> operator*(T scalar) const;
		friend inline Matrix<D, T> operator*(T scalar, const Matrix<D, T>& instance) { return instance * scalar; }
	};


#pragma region Details
	template<Dimension D, typename T>
	T Determinant(const Matrix<D, T>& instance)
	{
		static_assert( D >= 2 && D <= 4 );


		auto& M = instance;

		if constexpr ( D == 2 )
			// 2 terms(2!)
			return M[0][0] * M[1][1] - M[0][1] * M[1][0];

		if constexpr ( D == 3 )
			// 6 terms(3!)
			return
			M[0][0] * M[1][1] * M[2][2] + M[0][1] * M[1][2] * M[2][0] + M[0][2] * M[1][0] * M[2][1] -
			M[0][2] * M[1][1] * M[2][0] - M[0][1] * M[1][0] * M[2][2] - M[0][0] * M[1][2] * M[2][1];

		if constexpr ( D == 4 )
		{
			Matrix<3, T> M0 =
			{
				M[1][1], M[1][2], M[1][3],
				M[2][1], M[2][2], M[2][3],
				M[3][1], M[3][2], M[3][3],
			};

			Matrix<3, T> M1 =
			{
				M[1][0], M[1][2], M[1][3],
				M[2][0], M[2][2], M[2][3],
				M[3][0], M[3][2], M[3][3],
			};

			Matrix<3, T> M2 =
			{
				M[1][0], M[1][1], M[1][3],
				M[2][0], M[2][1], M[2][3],
				M[3][0], M[3][1], M[3][3],
			};

			Matrix<3, T> M3 =
			{
				M[1][0], M[1][1], M[1][2],
				M[2][0], M[2][1], M[2][2],
				M[3][0], M[3][1], M[3][2],
			};

			auto A0 = Power( -1, ( 1 + 1 ) ) * M[0][0] * Determinant(M0);
			auto A1 = Power( -1, ( 1 + 2 ) ) * M[0][1] * Determinant(M1);
			auto A2 = Power( -1, ( 1 + 3 ) ) * M[0][2] * Determinant(M2);
			auto A3 = Power( -1, ( 1 + 4 ) ) * M[0][3] * Determinant(M3);

			return A0 + A1 + A2 + A3;
		}
	}

	template<Dimension D, typename T>
	Matrix<D, T> Inverse(const Matrix<D, T>& instance)
	{
		static_assert( D >= 2 && D <= 4 );


		auto& M = instance;

		if constexpr ( D == 2 )
		{
			auto A00 = Power( -1, ( 1 + 1 ) ) * M[1][1];
			auto A01 = Power( -1, ( 1 + 2 ) ) * M[1][0];
			auto A10 = Power( -1, ( 2 + 1 ) ) * M[0][1];
			auto A11 = Power( -1, ( 2 + 2 ) ) * M[0][0];

			return 1 / Determinant(M) *
				Transpose(
				Matrix<2, T>(
					A00, A01,
					A10, A11
				)
				);
		}

		if constexpr ( D == 3 )
		{
			// row0
			Matrix<2, T> M00 =
			{
				M[1][1], M[1][2],
				M[2][1], M[2][2],
			};

			Matrix<2, T> M01 =
			{
				M[1][0], M[1][2],
				M[2][0], M[2][2],
			};

			Matrix<2, T> M02 =
			{
				M[1][0], M[1][1],
				M[2][0], M[2][1],
			};

			// row1
			Matrix<2, T> M10 =
			{
				M[0][1], M[0][2],
				M[2][1], M[2][2],
			};

			Matrix<2, T> M11 =
			{
				M[0][0], M[0][2],
				M[2][0], M[2][2],
			};

			Matrix<2, T> M12 =
			{
				M[0][0], M[0][1],
				M[2][0], M[2][1],
			};

			// row2
			Matrix<2, T> M20 =
			{
				M[0][1], M[0][2],
				M[1][1], M[1][2],
			};

			Matrix<2, T> M21 =
			{
				M[0][0], M[0][2],
				M[1][0], M[1][2],
			};

			Matrix<2, T> M22 =
			{
				M[0][0], M[0][1],
				M[1][0], M[1][1],
			};

			auto A00 = Power( -1, ( 1 + 1 ) ) * Determinant(M00);
			auto A01 = Power( -1, ( 1 + 2 ) ) * Determinant(M01);
			auto A02 = Power( -1, ( 1 + 3 ) ) * Determinant(M02);
			auto A10 = Power( -1, ( 2 + 1 ) ) * Determinant(M10);
			auto A11 = Power( -1, ( 2 + 2 ) ) * Determinant(M11);
			auto A12 = Power( -1, ( 2 + 3 ) ) * Determinant(M12);
			auto A20 = Power( -1, ( 3 + 1 ) ) * Determinant(M20);
			auto A21 = Power( -1, ( 3 + 2 ) ) * Determinant(M21);
			auto A22 = Power( -1, ( 3 + 3 ) ) * Determinant(M22);

			return 1 / Determinant(M) *
				Transpose(
				Matrix3(
					A00, A01, A02,
					A10, A11, A12,
					A20, A21, A22
				)
				);
		}

		if constexpr ( D == 4 )
		{
			// row0
			Matrix<3, T> M00 =
			{
				M[1][1], M[1][2], M[1][3],
				M[2][1], M[2][2], M[2][3],
				M[3][1], M[3][2], M[3][3],
			};

			Matrix<3, T> M01 =
			{
				M[1][0], M[1][2], M[1][3],
				M[2][0], M[2][2], M[2][3],
				M[3][0], M[3][2], M[3][3],
			};

			Matrix<3, T> M02 =
			{
				M[1][0], M[1][1], M[1][3],
				M[2][0], M[2][1], M[2][3],
				M[3][0], M[3][1], M[3][3],
			};

			Matrix<3, T> M03 =
			{
				M[1][0], M[1][1], M[1][2],
				M[2][0], M[2][1], M[2][2],
				M[3][0], M[3][1], M[3][2],
			};

			// row1
			Matrix<3, T> M10 =
			{
				M[0][1], M[0][2], M[0][3],
				M[2][1], M[2][2], M[2][3],
				M[3][1], M[3][2], M[3][3],
			};

			Matrix<3, T> M11 =
			{
				M[0][0], M[0][2], M[0][3],
				M[2][0], M[2][2], M[2][3],
				M[3][0], M[3][2], M[3][3],
			};

			Matrix<3, T> M12 =
			{
				M[0][0], M[0][1], M[0][3],
				M[2][0], M[2][1], M[2][3],
				M[3][0], M[3][1], M[3][3],
			};

			Matrix<3, T> M13 =
			{
				M[0][0], M[0][1], M[0][2],
				M[2][0], M[2][1], M[2][2],
				M[3][0], M[3][1], M[3][2],
			};

			// row2
			Matrix<3, T> M20 =
			{
				M[0][1], M[0][2], M[0][3],
				M[1][1], M[1][2], M[1][3],
				M[3][1], M[3][2], M[3][3],
			};

			Matrix<3, T> M21 =
			{
				M[0][0], M[0][2], M[0][3],
				M[1][0], M[1][2], M[1][3],
				M[3][0], M[3][2], M[3][3],
			};

			Matrix<3, T> M22 =
			{
				M[0][0], M[0][1], M[0][3],
				M[1][0], M[1][1], M[1][3],
				M[3][0], M[3][1], M[3][3],
			};

			Matrix<3, T> M23 =
			{
				M[0][0], M[0][1], M[0][2],
				M[1][0], M[1][1], M[1][2],
				M[3][0], M[3][1], M[3][2],
			};

			// row3
			Matrix<3, T> M30 =
			{
				M[0][1], M[0][2], M[0][3],
				M[1][1], M[1][2], M[1][3],
				M[2][1], M[2][2], M[2][3],
			};

			Matrix<3, T> M31 =
			{
				M[0][0], M[0][2], M[0][3],
				M[1][0], M[1][2], M[1][3],
				M[2][0], M[2][2], M[2][3],
			};

			Matrix<3, T> M32 =
			{
				M[0][0], M[0][1], M[0][3],
				M[1][0], M[1][1], M[1][3],
				M[2][0], M[2][1], M[2][3],
			};

			Matrix<3, T> M33 =
			{
				M[0][0], M[0][1], M[0][2],
				M[1][0], M[1][1], M[1][2],
				M[2][0], M[2][1], M[2][2],
			};

			auto A00 = Power( -1, ( 1 + 1 ) ) * Determinant(M00);
			auto A01 = Power( -1, ( 1 + 2 ) ) * Determinant(M01);
			auto A02 = Power( -1, ( 1 + 3 ) ) * Determinant(M02);
			auto A03 = Power( -1, ( 1 + 4 ) ) * Determinant(M03);

			auto A10 = Power( -1, ( 2 + 1 ) ) * Determinant(M10);
			auto A11 = Power( -1, ( 2 + 2 ) ) * Determinant(M11);
			auto A12 = Power( -1, ( 2 + 3 ) ) * Determinant(M12);
			auto A13 = Power( -1, ( 2 + 4 ) ) * Determinant(M13);

			auto A20 = Power( -1, ( 3 + 1 ) ) * Determinant(M20);
			auto A21 = Power( -1, ( 3 + 2 ) ) * Determinant(M21);
			auto A22 = Power( -1, ( 3 + 3 ) ) * Determinant(M22);
			auto A23 = Power( -1, ( 3 + 4 ) ) * Determinant(M23);

			auto A30 = Power( -1, ( 4 + 1 ) ) * Determinant(M30);
			auto A31 = Power( -1, ( 4 + 2 ) ) * Determinant(M31);
			auto A32 = Power( -1, ( 4 + 3 ) ) * Determinant(M32);
			auto A33 = Power( -1, ( 4 + 4 ) ) * Determinant(M33);

			return 1 / Determinant(M) *
				Transpose(
				Matrix4(
					A00, A01, A02, A03,
					A10, A11, A12, A13,
					A20, A21, A22, A23,
					A30, A31, A32, A33
				)
				);
		}
	}

	template<Dimension D, typename T>
	Matrix<D, T> Transpose(const Matrix<D, T>& instance)
	{
		Matrix<D, T> ret;
		for ( SizeType y = 0; y < D; y++ )
			for ( SizeType x = 0; x < D; x++ )
				ret[x][y] = instance[y][x];

		return ret;
	}

	template<Dimension D, typename T>
	Matrix<D, T>::Matrix()
	{

	}

	template<Dimension D, typename T>
	template<typename ...Parameters>
	Matrix<D, T>::Matrix(Parameters ...parameters)
	{
		static_assert( sizeof...(parameters) <= D * D );


		ParameterPackHelper::CopyTo( components, parameters... );
	}

	template<Dimension D, typename T>
	Matrix<D, T>::Matrix(const Matrix<D, T>& other)
	{
		Memory::Copy(
			other.components,
			components,
			sizeof(T) * D * D
		);
	}

	template<Dimension D, typename T>
	Void Matrix<D, T>::SetRow(SizeType index, const Vector<D, T>& vector)
	{
		rows[index] = vector;
	}

	template<Dimension D, typename T>
	Void Matrix<D, T>::SetColumn(SizeType index, const Vector<D, T>& vector)
	{
		for ( SizeType iteration = 0; iteration < D; iteration++ )
			rows[iteration][index] = vector[iteration];
	}

	template<Dimension D, typename T>
	const Vector<D, T>& Matrix<D, T>::GetRow(SizeType index) const
	{
		return rows[index];
	}

	template<Dimension D, typename T>
	Vector<D, T> Matrix<D, T>::GetColumn(SizeType index) const
	{
		Vector<D, T> ret;
		for ( SizeType iteration = 0; iteration < D; iteration++ )
			ret[iteration] = rows[iteration][index];

		return ret;
	}

	template<Dimension D, typename T>
	Matrix<D, T>& Matrix<D, T>::operator=(const Matrix<D, T>& other)
	{
		Memory::Copy(
			other.components,
			components,
			sizeof(T) * D * D
		);

		return *this;
	}

	template<Dimension D, typename T>
	Vector<D, T>& Matrix<D, T>::operator[](SizeType index)
	{
		return rows[index];
	}

	template<Dimension D, typename T>
	const Vector<D, T>& Matrix<D, T>::operator[](SizeType index) const
	{
		return rows[index];
	}

	template<Dimension D, typename T>
	Matrix<D, T> Matrix<D, T>::operator*(const Matrix<D, T>& other) const
	{
		Matrix<D, T> ret;
		for ( SizeType y = 0; y < D; y++ )
		{
			auto column = other.GetColumn(y);

			for ( SizeType x = 0; x < D; x++ )
				ret[x][y] = Dot( rows[x], column );
		}

		return ret;
	}

	template<Dimension D, typename T>
	Vector<D, T> Matrix<D, T>::operator*(const Vector<D, T>& vector) const
	{
		Vector<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = Dot( rows[index], vector );

		return ret;
	}

	template<Dimension D, typename T>
	Matrix<D, T> Matrix<D, T>::operator*(T scalar) const
	{
		Matrix<D, T> ret;
		for ( SizeType index = 0; index < D; index++ )
			ret[index] = rows[index] * scalar;

		return ret;
	}
#pragma endregion
}