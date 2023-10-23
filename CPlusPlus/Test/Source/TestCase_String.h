#pragma once

#include "Engine.h"

#include <iostream>


using namespace Engine;


inline Void TestCase_String()
{
	constexpr SizeType InSize = 128;
	Char8 InBuffer[InSize];

	{
		Bool parseBool;
		std::cout << "Parse Bool\n";
		if (TryParseBool(&parseBool, "false"))
			std::cout << "case0(false): " << parseBool << "\n";
		if (TryParseBool(&parseBool, "true"))
			std::cout << "case1(true): " << parseBool << "\n";

		Float32 parseFloat32;
		std::cout << "Parse Float32\n";
		if (TryParseFloat32(&parseFloat32, "0.00125"))
			std::cout << "case0(0.00125): " << parseFloat32 << "\n";
		if (TryParseFloat32(&parseFloat32, "-2.999"))
			std::cout << "case1(-2.999): " << parseFloat32 << "\n";

		Int32 parseInt32;
		std::cout << "Parse Int32\n";
		if (TryParseInt32(&parseInt32, "12345"))
			std::cout << "case0(12345): " << parseInt32 << "\n";
		if (TryParseInt32(&parseInt32, "-200"))
			std::cout << "case1(-200): " << parseInt32 << "\n";

		std::cout << "------\n";

		Bool inBool0 = false;
		Bool inBool1 = true;
		std::cout << "Convert Bool\n";
		if (TryConvertBool(InBuffer, InSize, inBool0))
			std::cout << "case0(false): " << InBuffer << "\n";
		if (TryConvertBool(InBuffer, InSize, inBool1))
			std::cout << "case1(true): " << InBuffer << "\n";

		Float32 inFloat320 = 0.0002255;
		Float32 inFloat321 = -1.24535;
		std::cout << "Convert Float32\n";
		if (TryConvertFloat32(InBuffer, InSize, inFloat320))
			std::cout << "case0(0.0002255): " << InBuffer << "\n";
		if (TryConvertFloat32(InBuffer, InSize, inFloat321))
			std::cout << "case1(-1.24535): " << InBuffer << "\n";

		Int32 inInt320 = 123458;
		Int32 inInt321 = -14762;
		std::cout << "Convert Int32\n";
		if (TryConvertInt32(InBuffer, InSize, inInt320))
			std::cout << "case0(123458): " << InBuffer << "\n";
		if (TryConvertInt32(InBuffer, InSize, inInt321))
			std::cout << "case1(-14762): " << InBuffer << "\n";
	}
}