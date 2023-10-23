#pragma once

#include "Engine.h"


using namespace Engine;


inline Void TestCase_BezierCurve()
{
	Bitmap bitmap(512, 512, Bitmap::Format::B8G8R8);
	Memory::Clear(
		bitmap.GetBuffer(),
		(bitmap.GetFormat() == Bitmap::Format::B8G8R8 ? 3 : 4) * bitmap.GetWidth() * bitmap.GetHeight()
	);

	constexpr SizeType Count = 5;
	Vector2 controlPoints[Count] =
	{
		{ 0, 0 },
		{ 128, 512 },
		{ 256, 0 },
		{ 384, 512 },
		{ 512, 0 },
	};


	constexpr RealType step = 0.01;
	for (RealType time = 0; time < 1; time += step)
	{
		Vector2 temporary[Count];
		Memory::Copy(controlPoints, temporary, sizeof(Vector2) * Count);

		Vector2 point = BezierCurve::Evaluate(temporary, Count, time);

		bitmap.SetPixel(
			point[0], point[1],
			(0xFF << 0) | (0xFF << 8) | (0xFF << 16)
		);
	}

	auto encoded = Bitmap::Encode(bitmap);

	U8String path = "bezier curve.bmp";
	if (File::Exist(path))
		File::Delete(path);

	File::WriteAllBytes(path, encoded.GetBuffer(), encoded.GetByteCount());
}