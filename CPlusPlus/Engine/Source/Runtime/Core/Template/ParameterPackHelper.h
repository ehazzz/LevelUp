#pragma once

#include "IsConvertible.h"



namespace Engine
{
	struct ParameterPackHelper
	{
		// Copy the value from parameter pack recursively(until it is empty) to a pointer point to heap/stack memory
		template<typename T, typename First, typename ...Parameters>
		static Void CopyTo(T* pointer, First value, Parameters ...parameters);
	};


#pragma region Details
	template<typename T, typename First, typename ...Parameters>
	Void ParameterPackHelper::CopyTo(T* pointer, First value, Parameters ...parameters)
	{
		static_assert( IsConvertible<First, T>::Value );

		
		*pointer = static_cast<T>(value);

		if constexpr ( sizeof...(parameters) > 0 )
			CopyTo( pointer + 1, parameters... );
	}
#pragma endregion
}