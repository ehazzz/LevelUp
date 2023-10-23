#pragma once



namespace Engine
{
	template<typename T>
	struct RemoveConst
	{
		using Type = T;
	};


	template<typename T>
	struct RemoveConst<const T>
	{
		using Type = T;
	};
}