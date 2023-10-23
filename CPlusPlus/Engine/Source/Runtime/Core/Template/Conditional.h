#pragma once

#include "../Base.h"



namespace Engine
{
	// Chooses between two different types based on a bool
	template<Bool Predicate, typename True, typename False>
	struct Conditional;


	template<typename True, typename False>
	struct Conditional<false, True, False>
	{
		using Type = False;
	};


	template<typename True, typename False>
	struct Conditional<true, True, False>
	{
		using Type = True;
	};
}