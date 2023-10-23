#include "Delay.h"

#include <thread>



namespace Engine
{
	Void Delay(SizeType milliseconds)
	{
		using Unit = std::chrono::milliseconds;


		std::this_thread::sleep_for( Unit(milliseconds) );
	}
}