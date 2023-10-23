#pragma once

// decode *.bmp
#include "../ThirdParty/Private/Bitmap.h"

// decode *.png
#include "../ThirdParty/lodepng/lodepng.h"

// decode *.jpg
#define STBI_ONLY_JPEG
#define STB_IMAGE_IMPLEMENTATION
#include "../ThirdParty/stb/stb_image.h"

#include <cmath>
#include <stdlib.h>



using namespace Engine;


class Image
{
public:
	enum class Format
	{
		BMP,
		PNG,
		JPG,
	};


	/*
	enum class PixelFormat
	{
		R8G8B8,
		R8G8B8A8,
		B8G8R8,
		B8G8R8A8,
	};
	*/


	template<Format From, Format To>
	static HeapMemory Convert(const Void* buffer, SizeType byteCount)
	{
		/**
		* Node:
		* use R8G8B8A8 as loaded image's pixel format
		*/


		Void* image = nullptr;
		SizeType imageSize;
		SizeType width;
		SizeType height;

		if constexpr (From == Format::BMP)
		{
			auto bitmap = Bitmap::Decode(buffer, byteCount);
			bitmap.FlipVertical();

			width = bitmap.GetWidth();
			height = bitmap.GetHeight();

			imageSize = 4 * width * height;
			image = Allocator::Allocate<Byte>(imageSize);

			for(SizeType y = 0; y < height; y++)
				for (SizeType x = 0; x < width; x++)
				{
					UInt32 value = bitmap.GetPixel(x, y);

					Byte b = (value >> 0) & 0xFF;
					Byte g = (value >> 8) & 0xFF;
					Byte r = (value >> 16) & 0xFF;
					Byte a = (value >> 24) & 0xFF;

					reinterpret_cast<UInt32*>(image)[width * y + x] =
						(r) |
						(g << 8) |
						(b << 16) |
						(a << 24);
				}
		}

		if constexpr (From == Format::PNG)
		{
			unsigned char* outputImage;
			unsigned int outputWidth;
			unsigned int outputHeight;

			lodepng_decode32(
				&outputImage,
				&outputWidth,
				&outputHeight,
				reinterpret_cast<const unsigned char*>(buffer),
				byteCount
			);

			width = outputWidth;
			height = outputHeight;
			
			imageSize = 4 * width * height;
			image = Allocator::Allocate<Byte>(imageSize);

			Memory::Copy(
				outputImage,
				image,
				imageSize
			);

			free(outputImage);
		}

		if constexpr (From == Format::JPG)
		{
			int outputWidth;
			int outputHeight;
			int outputComponents;

			auto outputImage = stbi_load_from_memory(
				reinterpret_cast<const stbi_uc*>(buffer),
				byteCount,
				&outputWidth,
				&outputHeight,
				&outputComponents,
				4
			);

			width = outputWidth;
			height = outputHeight;

			SizeType imageSize = 4 * width * height;
			Void* image = Allocator::Allocate<Byte>(imageSize);

			Memory::Copy(
				outputImage,
				image,
				imageSize
			);

			STBI_FREE(outputImage);
		}

		if constexpr (To == Format::BMP)
		{
			Bitmap bitmap(width, height, Bitmap::Format::B8G8R8A8);

			for (SizeType y = 0; y < height; y++)
				for (SizeType x = 0; x < width; x++)
				{
					UInt32 value = reinterpret_cast<UInt32*>(image)[width * y + x];

					Byte r = (value >> 0) & 0xFF;
					Byte g = (value >> 8) & 0xFF;
					Byte b = (value >> 16) & 0xFF;
					Byte a = (value >> 24) & 0xFF;

					bitmap.SetPixel(
						x, y,
						(b) | (g << 8) | (r << 16) | (a << 24)
					);
				}

			bitmap.FlipVertical();
			Allocator::Release(image);

			return Bitmap::Encode(bitmap);
		}

		if constexpr (To == Format::PNG)
		{
			unsigned char* encodedImage;
			size_t encodedImageSize;
			lodepng_encode32(&encodedImage, &encodedImageSize, reinterpret_cast<const unsigned char*>(image), width, height);

			HeapMemory heapMemory(encodedImageSize);
			Memory::Copy(
				encodedImage,
				heapMemory.GetBuffer(),
				encodedImageSize
			);

			free(encodedImage);
			Allocator::Release(image);

			return heapMemory;
		}

		if constexpr (To == Format::JPG)
		{
			// todo
		}
	}
};