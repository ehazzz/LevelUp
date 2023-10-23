#pragma once

#include "IsConvertible.h"
#include "IsEnum.h"



namespace Engine
{
	template<typename T>
	struct IsEnumClass : BoolConstant<IsEnum<T>::Value && !IsConvertible<T, Int32>::Value> { };
}