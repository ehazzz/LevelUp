#pragma once

/**
* Base Types Redefined
*/



namespace Engine
{
	using SByte		= char;

	using UByte		= unsigned char;

	using Int8		= SByte;
	using Int16		= short;
	using Int32		= int;
	using Int64		= long long;

	using UInt8		= UByte;
	using UInt16	= unsigned short;
	using UInt32	= unsigned int;
	using UInt64	= unsigned long long;

	using Float32	= float;
	using Float64	= double;

	using Char8		= SByte;
#if defined(_MSC_VER)
	using Char16	= wchar_t;
#else
#error Undefined symbol: Char16
#endif
	using Char32	= UInt32;

	using Bool		= bool;

	using Void		= void;


	using Byte		= UByte;

	using SizeType	= UInt64;
	
	using RealType	= Float32;

	using CodePoint	= Char32; // Used to represent unicode(0 - 0x10FFFF)
}