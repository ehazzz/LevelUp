#pragma once

#include "RemoveReference.h"



namespace Engine
{
	// Forward as lvalue
	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type& parameter);

	// Forward as rvalue
	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type&& parameter);

	
	// Forward parameter as movable
	template<typename T>
	constexpr typename RemoveReference<T>::Type&& Move(T&& parameter);


#pragma region Details
	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type& parameter)
	{
		return static_cast<T&&>(parameter);
	}

	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type&& parameter)
	{
		return static_cast<T&&>(parameter);
	}

	template<typename T>
	constexpr typename RemoveReference<T>::Type&& Move(T&& parameter)
	{
		return static_cast<typename RemoveReference<T>::Type&&>(parameter);
	}
#pragma endregion
}