#pragma once

#include "EnableIf.h"
#include "IsEnum.h"



namespace Engine
{
	template<typename T>
	using EnumUnderlyingType = typename EnableIf<IsEnum<T>::Value, __underlying_type(T)>::Type;
}