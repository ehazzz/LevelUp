#pragma once

#include "IsCharacter.h"
#include "IsFloatPoint.h"
#include "IsIntegral.h"



namespace Engine
{
	template<typename T>
	struct IsArithmetic : BoolConstant<IsCharacter<T>::Value || IsFloatPoint<T>::Value || IsIntegral<T>::Value> { };
}