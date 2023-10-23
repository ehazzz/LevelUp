#pragma once

#include "../Base.h"



typedef struct
{
	/**
	* Time point
	* 
	* 
	* @remark
	* Unit is millisecond
	*/
	SizeType start;
}
Timer;


struct FTimer
{
	// Record current system time
	Void(*Reset)(Timer* instance);


	// Measured in seconds
	RealType(*Elapsed)(const Timer* instance);
	SizeType(*ElapsedMilliseconds)(const Timer* instance);
};


extern ENGINE_API struct FTimer FTimer;