#pragma once

#include "ContainedFromParameterPack.h"



namespace Engine
{
	template<typename T>
	struct IsSignedInteger : BoolConstant<ContainedFromParameterPack<T, Int8, Int16, Int32, Int64>::Value> { };


	template<typename T>
	struct IsUnsignedInteger : BoolConstant<ContainedFromParameterPack<T, UInt8, UInt16, UInt32, UInt64>::Value> { };


	template<typename T>
	struct IsIntegral : BoolConstant<IsSignedInteger<T>::Value || IsUnsignedInteger<T>::Value> { };
}