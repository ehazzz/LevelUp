#pragma once

/**
* Used To Decode/Encode *.wav File
*/

#include "../Core.h"



namespace Engine
{
	class ENGINE_API Wave
	{
	public:
		static constexpr SizeType RIFFHeaderSize			= 12;
		static constexpr SizeType FormatBlockSize			= 24;
		static constexpr SizeType DataBlockSize				= 8;
		static constexpr SizeType DefaultAudioTrackOffset	= RIFFHeaderSize + FormatBlockSize + DataBlockSize;


		struct RIFFHeader
		{
			Byte	ID[4];		// Must be "RIFF"
			UInt32	size;		// The value should be the file size(*.wav) - 8
			Byte	format[4];	// Must be "WAVE"
		};


		struct FormatBlock
		{
			Byte	ID[4];	// Must be "fmt "
			UInt32	size;	// The value should be sizeof(FormatBlock) - 8
			UInt16	formatTag;
			UInt16	channelCount;
			UInt32	samplesPerSecond;
			UInt32	averageBytesPerSecond;
			UInt16	blockAlign;
			UInt16	bitsPerSample;
		};


		struct DataBlock
		{
			Byte	ID[4];	// Must be "data"
			UInt32	size;	// Indicate the size in bytes of the audio track
		};


		enum class FormatTag
		{
			PCM			= 1,
			IEEE_Float	= 3,
		};


		Wave();
		Wave(SizeType preallocated /** in bytes */, FormatTag formatTag, SizeType channelCount, SizeType samplesPerSecond, SizeType bitsPerSample);
		Wave(const Wave& other);
		Wave(Wave&& other);
		~Wave();


		Void SetAudioFrame(SizeType index, const Void* audioFrame);
		const Void* GetAudioFrame(SizeType index) const;

		
		// Test if a pointer point to heap memory is a valid *.wav file representation
		static Bool IsValid(const Void* buffer, SizeType byteCount);

		static Wave Decode(const Void* buffer, SizeType byteCount);
		static HeapMemory Encode(const Wave& instance);


		Void* GetBuffer();
		const Void* GetBuffer() const;
		SizeType GetByteCount() const;
		FormatTag GetFormatTag() const;
		SizeType GetChannelCount() const;
		SizeType GetSamplesPerSecond() const;
		SizeType GetBitsPerSample() const;
		SizeType GetAudioFrameSize() const;


		Wave& operator=(const Wave& other);
		Wave& operator=(Wave&& other);

		Void* operator[](SizeType index);
		const Void* operator[](SizeType index) const;
	private:
		Void CopyFrom(const Wave& instance);
	private:
		Void*		buffer;
		SizeType	byteCount;

		FormatTag	formatTag;
		SizeType	channelCount;
		SizeType	samplesPerSecond;	// Indicate how many samples had been used in a second
		SizeType	bitsPerSample;		// Indicate how many bits used by a sample(a L/R audio frame)
		SizeType	audioFrameSize;
	};
}