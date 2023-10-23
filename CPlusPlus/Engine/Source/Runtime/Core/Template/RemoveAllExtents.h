#pragma once

#include "../Base.h"



namespace Engine
{
	template<typename T>
	struct RemoveAllExtents
	{
		using Type = T;
	};


	template<typename T>
	struct RemoveAllExtents<T[]>
	{
		using Type = typename RemoveAllExtents<T>::Type;
	};


	template<SizeType Size, typename T>
	struct RemoveAllExtents<T[Size]>
	{
		using Type = typename RemoveAllExtents<T>::Type;
	};
}