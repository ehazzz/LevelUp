#pragma once

#include "../../Asset/Bitmap.h"



namespace Engine::Windows
{
	class ENGINE_API Icon
	{
	public:
		Icon();
		/**
		* @remark
		* xHotspot and yHotspot are ignored if !asSystemCursor
		*/
		Icon(const Bitmap& bitmap, Bool asSystemCursor = false, SizeType xHotspot = 0, SizeType yHotspot = 0);
		Icon(const Char16* executableFilePath);
		~Icon();


		Void AttachToWindow(Void* nativeWindowsWindowHandle);
		Void SetAsSystemCursor();

		Bitmap ExtractToBitmap();


		Void* GetHandle() const;
	private:
		Void* handle;
	};
}