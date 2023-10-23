#pragma once

#include "../Base.h"



namespace Engine
{
	template<typename T>
	struct RemoveExtent
	{
		using Type = T;
	};


	template<typename T>
	struct RemoveExtent<T[]>
	{
		using Type = T;
	};


	template<SizeType Size, typename T>
	struct RemoveExtent<T[Size]>
	{
		using Type = T;
	};
}