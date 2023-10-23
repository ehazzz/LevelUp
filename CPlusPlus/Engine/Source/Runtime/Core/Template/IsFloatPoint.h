#pragma once

#include "ContainedFromParameterPack.h"



namespace Engine
{
	template<typename T>
	struct IsFloatPoint : BoolConstant<ContainedFromParameterPack<T, Float32, Float64>::Value> { };
}