#pragma once

#include "../Base.h"



namespace Engine
{
	template<typename T, T InValue>
	struct IntegralConstant
	{
		static constexpr T Value = InValue;
	};


	template<Bool InValue>
	using BoolConstant = IntegralConstant<bool, InValue>;

	using FalseType	= BoolConstant<false>;
	using TrueType	= BoolConstant<true>;
}