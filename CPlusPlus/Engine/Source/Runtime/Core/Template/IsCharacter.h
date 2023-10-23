#pragma once

#include "ContainedFromParameterPack.h"



namespace Engine
{
	template<typename T>
	struct IsCharacter : BoolConstant<ContainedFromParameterPack<T, Char8, Char16, Char32>::Value> { };
}