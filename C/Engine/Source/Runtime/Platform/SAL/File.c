#include "File.h"

#if defined(PLATFORM_WINDOWS)
#include "../Windows/Windows.h"
#endif



static File Construct(const U8String* path, FileOpenOption option)
{
#if defined(PLATFORM_WINDOWS)
	UInt32 access;
	UInt32 creation;
	switch (option)
	{
	case FileOpenOption_Read:
		access		= GENERIC_READ;
		creation	= OPEN_EXISTING;
		break;
	case FileOpenOption_Write:
		access		= GENERIC_WRITE;
		creation	= CREATE_NEW;
		break;
	}

	U16String u16 = FU8String.ToU16String(path);
	HANDLE handle = CreateFileW( u16.buffer, access, 0, NullPointer, creation, FILE_ATTRIBUTE_NORMAL, NullPointer );

	FU16String.Destruct( &u16 );

	File ret = { handle };
	return ret;
#else
#error No implementation
#endif
}

static Void Destruct(File* instance)
{
#if defined(PLATFORM_WINDOWS)
	CloseHandle( CAST( HANDLE, instance->handle ) );
#else
#error No implementation
#endif
}

static Void Seek(File* instance, FileSeekOption option, Int64 offset)
{
#if defined(PLATFORM_WINDOWS)
	UInt32 method;
	switch (option)
	{
	case FileSeekOption_Begin:
		method = FILE_BEGIN;
		break;
	case FileSeekOption_Current:
		method = FILE_CURRENT;
		break;
	case FileSeekOption_End:
		method = FILE_END;
		break;
	}

	LARGE_INTEGER distance;
	distance.QuadPart = offset;

	SetFilePointerEx(
		CAST( HANDLE, instance->handle ),
		distance,
		NullPointer,
		method
	);
#else
#error No implementation
#endif
}

static Void Read(File* instance, Void* buffer, SizeType byteCount)
{
#if defined(PLATFORM_WINDOWS)
	ReadFile(
		CAST( HANDLE, instance->handle ),
		buffer,
		byteCount,
		NullPointer,
		NullPointer
	);
#else
#error No implementation
#endif
}

static Void Write(File* instance, const Void* buffer, SizeType byteCount)
{
#if defined(PLATFORM_WINDOWS)
	WriteFile(
		CAST( HANDLE, instance->handle ),
		buffer,
		byteCount,
		NullPointer,
		NullPointer
	);
#else
#error No implementation
#endif
}

static Void Copy(const U8String* from, const U8String* to)
{
	HeapMemory loaded = FFile.ReadAllBytes(from);
	FFile.WriteAllBytes( to, loaded.buffer, loaded.byteCount );

	FHeapMemory.Destruct( &loaded );
}

static Void Delete(const U8String* path)
{
#if defined(PLATFORM_WINDOWS)
	U16String u16 = FU8String.ToU16String(path);
	DeleteFileW( u16.buffer );

	FU16String.Destruct( &u16 );
#else
#error No implementation
#endif
}

static Bool Exist(const U8String* path)
{
#if defined(PLATFORM_WINDOWS)
	File file = FFile.Construct( path, FileOpenOption_Read );

	if ( GetLastError() == ERROR_FILE_NOT_FOUND )
		return False;

	FFile.Destruct( &file );

	U16String u16	= FU8String.ToU16String(path);
	Bool isFile		= ( GetFileAttributesW( u16.buffer ) & FILE_ATTRIBUTE_DIRECTORY ) != FILE_ATTRIBUTE_DIRECTORY;

	FU16String.Destruct( &u16 );

	if (isFile)
		return True;

	return False;
#else
#error No implementation
#endif
}

static SizeType GetSize(const U8String* path)
{
#if defined(PLATFORM_WINDOWS)
	File file = FFile.Construct( path, FileOpenOption_Read );

	LARGE_INTEGER ret;
	GetFileSizeEx( CAST( HANDLE, file.handle ), &ret );

	FFile.Destruct( &file );

	return ret.QuadPart;
#else
#error No implementation
#endif
}

static Void Move(const U8String* from, const U8String* to)
{
#if defined(PLATFORM_WINDOWS)
	U16String u16From	= FU8String.ToU16String(from);
	U16String u16To		= FU8String.ToU16String(to);

	MoveFileW( u16From.buffer, u16To.buffer );

	FU16String.Destruct( &u16To );
	FU16String.Destruct( &u16From );
#else
#error No implementation
#endif
}

static HeapMemory ReadAllBytes(const U8String* path)
{
	HeapMemory ret = FHeapMemory.Construct( FFile.GetSize(path) );

	File file = FFile.Construct( path, FileOpenOption_Read );
	FFile.Read( &file, ret.buffer, ret.byteCount );

	FFile.Destruct( &file );

	return ret;
}

static Void WriteAllBytes(const U8String* path, const Void* buffer, SizeType byteCount)
{
	File file = FFile.Construct( path, FileOpenOption_Write );
	FFile.Write( &file, buffer, byteCount );

	FFile.Destruct( &file );
}


struct FFile FFile = 
{
	Construct,
	Destruct,
	Seek,
	Read,
	Write,
	Copy,
	Delete,
	Exist,
	GetSize,
	Move,
	ReadAllBytes,
	WriteAllBytes,
};
