#include "Allocator.h"

#if defined(PLATFORM_WINDOWS)
#include "../../Platform/Windows/Windows.h"
#endif



static Void* Allocate(SizeType byteCount)
{
#if defined(PLATFORM_WINDOWS)
	return HeapAlloc( GetProcessHeap(), 0, byteCount );
#else
#error No implementation
#endif
}

static Void Release(Void* pointer)
{
#if defined(PLATFORM_WINDOWS)
	HeapFree( GetProcessHeap(), 0, pointer );
#else
#error No implementation
#endif
}


struct FAllocator FAllocator =
{
	Allocate,
	Release,
};