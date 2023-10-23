#pragma once

#include "../../Core.h"



struct FFileDialog
{
	/**
	* Open a file dialog and selected filename(absolute path) will be placed in "inBuffer"
	* 
	* 
	* @remark
	* Filter samples
	* Select all files				- L"(*.*)\0*.*\0"
	* Select *.txt file				- L"(*.txt)\0*.txt\0"
	* Select *.bmp and *.png files	- L"(*.bmp)\0*.bmp\0(*.png)\0*.png\0"
	* 
	* 
	* @param inBuffer	A pointer point to allocated heap/stack memory
	* @param inSize		Indicate how many Char16s could be used
	*/
	Bool(*Open)(Char16* inBuffer, SizeType inSize, const Char16* filters, const Char16* initDirectory, const Char16* title, Void* owner);
	Bool(*Save)(Char16* inBuffer, SizeType inSize, const Char16* filters, const Char16* initDirectory, const Char16* title, Void* owner);
};


extern ENGINE_API struct FFileDialog FFileDialog;