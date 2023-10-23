#pragma once

#include "IntegralConstant.h"



namespace Engine
{
	template<typename T>
	struct IsEnum : BoolConstant<__is_enum(T)> { };
}