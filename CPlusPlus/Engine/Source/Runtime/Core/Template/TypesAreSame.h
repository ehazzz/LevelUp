#pragma once

#include "IntegralConstant.h"



namespace Engine
{
	// Test if two or more types are same
	template<typename ...Ts>
	struct TypesAreSame;


	template<typename T>
	struct TypesAreSame<T, T> : TrueType { };


	template<typename First, typename Second>
	struct TypesAreSame<First, Second> : FalseType { };


	template<typename First, typename Second, typename ...Ts>
	struct TypesAreSame<First, Second, Ts ...> : BoolConstant<TypesAreSame<First, Second>::Value && TypesAreSame<Second, Ts ...>::Value> { };
}