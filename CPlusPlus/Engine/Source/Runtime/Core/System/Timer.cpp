#include "Timer.h"

#include <chrono>
#include <thread>



namespace Engine
{
	static SizeType CurrentSystemTime()
	{
		using Unit	= std::chrono::milliseconds;
		using Clock	= std::chrono::high_resolution_clock;


		auto duration = Clock::now().time_since_epoch();

		return std::chrono::duration_cast<Unit>(duration).count();
	}


	Timer::Timer()
	{
		Reset();
	}

	Void Timer::Reset()
	{
		start = CurrentSystemTime();
	}

	RealType Timer::Elapsed() const
	{
		return ElapsedMilliseconds() / 1000.0;
	}

	SizeType Timer::ElapsedMilliseconds() const
	{
		return CurrentSystemTime() - start;
	}
}