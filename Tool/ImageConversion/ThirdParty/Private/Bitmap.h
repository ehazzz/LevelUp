#pragma once

/**
* Used To Decode/Encode Uncompressed 24-bit(B8G8R8) Or 32-bit(B8G8R8A8) *.bmp File
*/

#include "HeapMemory.h"



namespace Engine
{
	class Bitmap
	{
	public:
		static constexpr SizeType FileHeaderSize	= 14;
		static constexpr SizeType InfoHeaderSize	= 40;
		static constexpr SizeType DefaultBitsOffset	= FileHeaderSize + InfoHeaderSize;


		/**
		* Reference from https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
		* 
		* 
		* @remark
		* The size in bytes of the struct must be 14
		*/
		#pragma pack(push, 2)
		struct FileHeader
		{
			Byte	magic[2];
			UInt32	size;
			UInt32	reserved;
			UInt32	offset;
		};
		#pragma pack(pop)


		/**
		* Reference from https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
		*
		*
		* @remark
		* The size in bytes of the struct should be 40
		*/
		struct InfoHeader
		{
			UInt32 size;
			UInt32 width;
			UInt32 height;
			UInt16 planes;
			UInt16 bitCount;
			UInt32 compression;
			UInt32 imageSize;
			UInt32 xPixelsPerMeter;
			UInt32 yPixelsPerMeter;
			UInt32 colorUsed;
			UInt32 colorImportant;
		};


		enum class Format
		{
			Unknown,

			B8G8R8,
			B8G8R8A8,
		};


		Bitmap();
		Bitmap(SizeType width, SizeType height, Format format);
		Bitmap(const Bitmap& other);
		Bitmap(Bitmap&& other);
		~Bitmap();


		Void SetPixel(SizeType x, SizeType y, UInt32 pixel);
		UInt32 GetPixel(SizeType x, SizeType y) const;

		Void FlipHorizontal();
		Void FlipVertical();


		// Test if a pointer point to heap memory is a valid *.bmp file representation
		static Bool IsValid(const Void* buffer, SizeType byteCount);

		static Bitmap Decode(const Void* buffer, SizeType byteCount);
		static HeapMemory Encode(const Bitmap& instance);

		
		Void* GetBuffer();
		const Void* GetBuffer() const;
		SizeType GetWidth() const;
		SizeType GetHeight() const;
		Format GetFormat() const;
		SizeType GetChannelCount() const;


		Bitmap& operator=(const Bitmap& other);
		Bitmap& operator=(Bitmap&& other);
	private:
		Void CopyFrom(const Bitmap& instance);
	private:
		Void*		buffer;
		SizeType	width;
		SizeType	height;
		Format		format;
		SizeType	channelCount;
	};
}