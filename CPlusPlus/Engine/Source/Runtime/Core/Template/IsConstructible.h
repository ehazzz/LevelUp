#pragma once

#include "IntegralConstant.h"



namespace Engine
{
	// Test if a instance of typename T could be constructed with some special parameters
	template<typename T, typename ...Parameters>
	struct IsConstructible : BoolConstant<__is_constructible(T, Parameters ...)> { };
}