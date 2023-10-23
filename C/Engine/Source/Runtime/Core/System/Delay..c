#include "Delay.h"

#if defined(PLATFORM_WINDOWS)
#include "../../Platform/Windows/Windows.h"
#endif



static Void Delay(SizeType milliseconds)
{
#if defined(PLATFORM_WINDOWS)
	Sleep(milliseconds);
#else
#error No implementation
#endif
}


struct FDelay FDelay =
{
	Delay,
};