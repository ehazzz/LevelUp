#include "Timer.h"

#if defined(PLATFORM_WINDOWS)
#include "../../Platform/Windows/Windows.h"
#endif



static SizeType CurrentSystemTime()
{
#if defined(PLATFORM_WINDOWS)
	static Float64 period = -1;
	if ( period < 0 )
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency( &frequency );

		period = 1.0 / frequency.QuadPart;
	}

	LARGE_INTEGER counter;
	QueryPerformanceCounter( &counter );

	return period * counter.QuadPart * 1000;
#else
#error No implementation
#endif
}


static Void Reset(Timer* instance)
{
	instance->start = CurrentSystemTime();
}

static RealType Elapsed(const Timer* instance)
{
	return FTimer.ElapsedMilliseconds(instance) / 1000.0;
}

static SizeType ElapsedMilliseconds(const Timer* instance)
{
	return CurrentSystemTime() - instance->start;
}


struct FTimer FTimer =
{
	Reset,
	Elapsed,
	ElapsedMilliseconds,
};