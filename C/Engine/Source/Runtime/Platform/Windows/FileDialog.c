#include "FileDialog.h"

#include "Windows.h"



typedef BOOL(APIENTRY*FileDialogCallback)(OPENFILENAMEW*);


static Bool OpenDialog(Char16* inBuffer, SizeType inSize, const Char16* filters, const Char16* initDirectory, const Char16* title, Void* owner, FileDialogCallback Fn)
{
	FMemory.Clear( inBuffer, sizeof(Char16) * inSize );


	OPENFILENAMEW descriptor;
	FMemory.Clear( &descriptor, sizeof(OPENFILENAMEW) );
	descriptor.lStructSize		= sizeof(OPENFILENAMEW);
	descriptor.hwndOwner		= CAST(HWND, owner);
	descriptor.lpstrFilter		= filters;
	descriptor.lpstrFile		= inBuffer;
	descriptor.nMaxFile			= inSize;
	descriptor.lpstrInitialDir	= initDirectory;
	descriptor.lpstrTitle		= title;
	descriptor.Flags			= OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return Fn( &descriptor );
}


static Bool Open(Char16* inBuffer, SizeType inSize, const Char16* filters, const Char16* initDirectory, const Char16* title, Void* owner)
{
	return OpenDialog(inBuffer, inSize, filters, initDirectory, title, owner, GetOpenFileNameW);
}

static Bool Save(Char16* inBuffer, SizeType inSize, const Char16* filters, const Char16* initDirectory, const Char16* title, Void* owner)
{
	return OpenDialog(inBuffer, inSize, filters, initDirectory, title, owner, GetSaveFileNameW);
}


struct FFileDialog FFileDialog = 
{
	Open,
	Save,
};
