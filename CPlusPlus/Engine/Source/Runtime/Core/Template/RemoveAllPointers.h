#pragma once



namespace Engine
{
	template<typename T>
	struct RemoveAllPointers
	{
		using Type = T;
	};


	template<typename T>
	struct RemoveAllPointers<T*>
	{
		using Type = typename RemoveAllPointers<T>::Type;
	};
}