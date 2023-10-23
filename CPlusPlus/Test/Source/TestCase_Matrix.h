#pragma once

#include "Engine.h"



using namespace Engine;


inline Void TestCase_Matrix()
{
	{
		Matrix2 M =
		{
			1, 2,
			3, 4,
		};


		auto determinant = Determinant(M);

		auto M_I = Inverse(M);

		auto result = M * M_I;

		int breakpoint = -1;
	}

	{
		Matrix3 M =
		{
			//1, 2, 3,
			0, 2, 3,
			4, 5, 6,
			7, 8, 9,
		};

		auto determinant = Determinant(M);

		auto M_I = Inverse(M);

		auto result = M * M_I;

		int breakpoint = -1;
	}
	
	{
		Matrix4 M =
		{
			1, 2, 3, 4,
			2, 2, 3, 4,
			3, 3, 4, 0,
			4, 4, 0, 5,
		};

		auto determinant = Determinant(M);

		auto M_I = Inverse(M);

		auto result = M * M_I;

		int breakpoint = -1;
	}
}