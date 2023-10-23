#pragma once

#include "IntegralConstant.h"



namespace Engine
{
	// Test if a type could be converted to the other
	template<typename From, typename To>
	struct IsConvertible : BoolConstant<__is_convertible_to(From, To)> { };
}