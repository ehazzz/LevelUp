#include "Directory.h"

#if defined(PLATFORM_WINDOWS)
#include "../Windows/Windows.h"
#endif



namespace Engine
{
    Void Directory::Copy(const U8String& from, const U8String& to)
    {
        if ( !Directory::Exist(to) )
            Directory::Create(to);

        for ( auto& entry : EnumEntries( from, SearchOption::File | SearchOption::Directory ) )
        {
            auto combinedFrom   = Path::Combine(from,   entry);
            auto combinedTo     = Path::Combine(to,     entry);

            if ( File::Exist(combinedFrom) )
                File::Copy(combinedFrom, combinedTo);

            if ( Directory::Exist(combinedFrom) )
                Directory::Copy(combinedFrom, combinedTo);
        }
    }

    Void Directory::Create(const U8String& path)
    {
    #if defined(PLATFORM_WINDOWS)
        CreateDirectoryW( static_cast<U16String>(path), nullptr );
    #else
    #error No implementation
    #endif
    }

    Void Directory::Delete(const U8String& path)
    {
    #if defined(PLATFORM_WINDOWS)
        for ( auto& entry : EnumEntries( path, SearchOption::File | SearchOption::Directory ) )
        {
            auto combined = Path::Combine(path, entry);

            if ( File::Exist(combined) )
                File::Delete(combined);

            if ( Directory::Exist(combined) )
                Directory::Delete(combined);
        }

        RemoveDirectoryW( static_cast<U16String>(path) );
    #else
    #error No implementation
    #endif
    }

    Bool Directory::Exist(const U8String& path)
    {
    #if defined(PLATFORM_WINDOWS)
        File file( path, File::OpenOption::Read );

        if ( GetLastError() == ERROR_FILE_NOT_FOUND )
            return false;

        if (
            GetFileAttributesW( static_cast<U16String>(path) ) & FILE_ATTRIBUTE_DIRECTORY
            )
            return true;

        return false;
    #else
    #error No implementation
    #endif
    }

    Void Directory::Move(const U8String& from, const U8String& to)
    {
    #if defined(PLATFORM_WINDOWS)
        File::Move(from, to);
    #else
    #error No implementation
    #endif
    }

    DynamicArray<U8String> Directory::EnumEntries(const U8String& path, SearchOption option)
    {
    #if defined(PLATFORM_WINDOWS)
        WIN32_FIND_DATAW data;
        HANDLE handle = FindFirstFileW(
            static_cast<U16String>( Path::Combine(path, "*.*") ),
            &data
        );

        if ( handle == INVALID_HANDLE_VALUE )
            return DynamicArray<U8String>();

        DynamicArray<U8String> ret;
        do
        {
            // ignore default entries("." and "..")
            if (
                Memory::Equal( data.cFileName, L".",     sizeof(Char16) * 2 ) ||
                Memory::Equal( data.cFileName, L"..",    sizeof(Char16) * 3 )
                )
                continue;

            Bool selectFile         = ( option & SearchOption::File      ) == SearchOption::File;
            Bool selectDirectory    = ( option & SearchOption::Directory ) == SearchOption::Directory;

            Bool isFile         = ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != FILE_ATTRIBUTE_DIRECTORY;
            Bool isDirectory    = !isFile;

            if (
                ( selectFile        && isFile       ) ||
                ( selectDirectory   && isDirectory  )
                )
                ret.Add( U16String( data.cFileName ) );

        } while ( FindNextFileW( handle, &data ) );
        FindClose(handle);

        return ret;
    #else
    #error No implementation
    #endif
    }
}