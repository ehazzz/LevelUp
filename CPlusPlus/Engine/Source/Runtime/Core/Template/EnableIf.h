#pragma once

#include "../Base.h"



namespace Engine
{
	// No member Type if !Predicate
	template<Bool Predicate, typename T>
	struct EnableIf;


	template<typename T>
	struct EnableIf<false, T>
	{

	};


	template<typename T>
	struct EnableIf<true, T>
	{
		using Type = T;
	};
}