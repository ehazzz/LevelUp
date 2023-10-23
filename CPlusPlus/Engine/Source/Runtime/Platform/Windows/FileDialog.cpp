#include "FileDialog.h"

#include "Windows.h"



namespace Engine::Windows
{
	using FileDialogCallback = BOOL(APIENTRY*)(OPENFILENAMEW*);


	static Bool OpenDialog(Char16* inBuffer, SizeType inSize, const Char16* filters, const Char16* initialDirectory, const Char16* title, Void* owner, FileDialogCallback Fn)
	{
		Memory::Clear( inBuffer, sizeof(Char16) * inSize );

		OPENFILENAMEW descriptor;
		Memory::Clear( &descriptor, sizeof(OPENFILENAMEW) );
		descriptor.lStructSize		= sizeof(OPENFILENAMEW);
		descriptor.hwndOwner		= reinterpret_cast<HWND>(owner);
		descriptor.lpstrFilter		= filters;
		descriptor.lpstrFile		= inBuffer;
		descriptor.nMaxFile			= inSize;
		descriptor.lpstrInitialDir	= initialDirectory;
		descriptor.lpstrTitle		= title;
		descriptor.Flags			= OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		return Fn( &descriptor ) != 0;
	}


	Bool FileDialog::Open(Char16* inBuffer, SizeType inSize, const Char16* filters, const Char16* initialDirectory, const Char16* title, Void* owner)
	{
		return OpenDialog(inBuffer, inSize, filters, initialDirectory, title, owner, GetOpenFileNameW);
	}

	Bool FileDialog::Save(Char16* inBuffer, SizeType inSize, const Char16* filters, const Char16* initialDirectory, const Char16* title, Void* owner)
	{
		return OpenDialog(inBuffer, inSize, filters, initialDirectory, title, owner, GetSaveFileNameW);
	}
}