#pragma once

#include "../../Core.h"



namespace Engine::Windows
{
	class ENGINE_API FileDialog
	{
	public:
		/**
		* Open a file dialog and selected filename(absolute path) will be placed in [inBuffer]
		* 
		* 
		* @param inBuffer	As a output parameter(a pointer point to allocated heap/stack memory), and selected filename will be placed in
		* @param inSize		Indicate how many Char16s could be used
		* 
		* @return True if successed to execute otherwise false
		*/
		static Bool Open(Char16* inBuffer, SizeType inSize, const Char16* filters = L"(*.*)\0*.*\0", const Char16* initialDirectory = L"./", const Char16* title = L"Open file", Void* owner = nullptr /** A native Windows window handle */);
		static Bool Save(Char16* inBuffer, SizeType inSize, const Char16* filters = L"(*.*)\0*.*\0", const Char16* initialDirectory = L"./", const Char16* title = L"Save as..", Void* owner = nullptr /** A native Windows window handle */);
	};
}