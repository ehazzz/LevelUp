#pragma once

#include "../../Core.h"



struct FPath
{
	// Combine two paths to form a new path
	U8String(*Combine)(const U8String* A, const U8String* B);
};

extern ENGINE_API struct FPath FPath;