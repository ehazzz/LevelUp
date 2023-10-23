#pragma once

#include "../../Core.h"

#include "../SAL/EventSystem.h"



namespace Engine::Windows
{
	class ENGINE_API Window
	{
	public:
		NONCOPYABLE(Window);


		Window();
		~Window();


		// Create a Windows window and display it on desktop
		Void Create(const Char16* title = L"Default Window", SizeType width = 1280, SizeType height = 720, Void* userParameter = nullptr);
		// Poll events from OS
		Void Update();

		Void SetTitle(const Char16* title);
		Void SetIsResizableWindow(Bool isResizable);

		Void SetWidth(SizeType width);
		Void SetHeight(SizeType height);


		Void* GetHandle() const;
		SizeType GetWidth() const;
		SizeType GetHeight() const;
	private:
		Void* handle;

		SizeType width;
		SizeType height;
	};
}