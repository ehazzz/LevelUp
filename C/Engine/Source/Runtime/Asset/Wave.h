#pragma once

/**
* Used To Decode/Encode *.wav File
*/

#include "../Core.h"



typedef struct
{
	Byte	ID[4];		// Must be "RIFF"
	UInt32	size;		// The value should be the file size(*.wav file) - 8
	Byte	format[4];	// Must be "WAVE"
}
WaveRIFFHeader;


typedef struct
{
	Byte	ID[4];	// Must be "fmt "
	UInt32	size;	// The value should be sizeof(FormatBlock) - 8
	UInt16	formatTag;
	UInt16	channelCount;
	UInt32	samplesPerSecond;
	UInt32	averageBytesPerSecond;
	UInt16	blockAlign;
	UInt16	bitsPerSample;
}
WaveFormatBlock;


typedef struct
{
	Byte	ID[4];	// Must be "data"
	UInt32	size;	// Indicate the size in bytes of audio track
}
WaveDataBlock;


typedef enum
{
	WaveFormatTag_PCM	= 1,
	WaveFormatTag_Float	= 3,
}
WaveFormatTag;


typedef struct
{
	Void*		buffer;
	SizeType	byteCount;

	WaveFormatTag	formatTag;
	SizeType		channelCount;
	SizeType		samplesPerSecond;	// Indciate how many samples used in a second
	SizeType		bitsPerSample;		// Indicate how many bits used by a sample(a L/R audio frame)
	SizeType		audioFrameSize;
}
Wave;


struct FWave
{
	Wave(*Construct)(SizeType byteCount, WaveFormatTag formatTag, SizeType channelCount, SizeType samplesPerSecond, SizeType bitsPerSample);
	Void(*Destruct)(Wave* instance);


	Void(*SetAudioFrame)(Wave* instance, SizeType index, const Void* audioFrame);
	const Void*(*GetAudioFrame)(const Wave* instance, SizeType index);


	Bool(*IsValid)(const Void* buffer, SizeType byteCount);

	Wave(*Decode)(const Void* buffer, SizeType byteCount);
	HeapMemory(*Encode)(const Wave* instance);
};


extern ENGINE_API struct FWave FWave;