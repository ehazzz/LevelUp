#pragma once

#include "../Base.h"



struct FDelay
{
	Void(*Delay)(SizeType milliseconds);
};


extern ENGINE_API struct FDelay FDelay;