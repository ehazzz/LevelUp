#include "Wave.h"



static const SizeType WaveRIFFHeaderSize			= sizeof(WaveRIFFHeader);
static const SizeType WaveFormatBlockSize			= sizeof(WaveFormatBlock);
static const SizeType WaveDataBlockSize				= sizeof(WaveDataBlock);
static const SizeType WaveDefaultAudioTrackOffset	= sizeof(WaveRIFFHeader) + sizeof(WaveFormatBlock) + sizeof(WaveDataBlock);


static Wave Construct(SizeType byteCount, WaveFormatTag formatTag, SizeType channelCount, SizeType samplesPerSecond, SizeType bitsPerSample)
{
	Wave ret;
	ret.buffer				= FAllocator.Allocate(byteCount);
	ret.byteCount			= byteCount;
	ret.formatTag			= formatTag;
	ret.channelCount		= channelCount;
	ret.samplesPerSecond	= samplesPerSecond;
	ret.bitsPerSample		= bitsPerSample;
	ret.audioFrameSize		= bitsPerSample / 8 * channelCount;

	return ret;
}

static Void Destruct(Wave* instance)
{
	FAllocator.Release( instance->buffer );
}

static Void SetAudioFrame(Wave* instance, SizeType index, const Void* audioFrame)
{
	FMemory.Copy(
		audioFrame,
		CAST( Byte*, instance->buffer ) + instance->audioFrameSize * index,
		instance->audioFrameSize
	);
}

static const Void* GetAudioFrame(const Wave* instance, SizeType index)
{
	return CAST( Byte*, instance->buffer ) + instance->audioFrameSize * index;
}

static Bool IsValid(const Void* buffer, SizeType byteCount)
{
	if ( byteCount < WaveDefaultAudioTrackOffset )
		return False;

	{
		const WaveRIFFHeader* RIFF = CAST( const WaveRIFFHeader*, buffer );

		if ( !FMemory.Equal( RIFF->ID, "RIFF", 4 ) )
			return False;

		if ( byteCount != RIFF->size + 8 )
			return False;

		if ( !FMemory.Equal( RIFF->format, "WAVE", 4 ) )
			return False;
	}

	{
		const WaveFormatBlock* format = REINTERPRET_CAST(const WaveFormatBlock*, buffer, WaveRIFFHeaderSize);

		if ( !FMemory.Equal( format->ID, "fmt ", 4 ) )
			return False;

		if (
			format->formatTag != WaveFormatTag_PCM &&
			format->formatTag != WaveFormatTag_Float
			)
			return False;

		if (
			format->channelCount != 1 &&
			format->channelCount != 2
			)
			return False;
	}

	return True;
}

static Wave Decode(const Void* buffer, SizeType byteCount)
{
	SizeType offset = WaveRIFFHeaderSize;

	const WaveFormatBlock* format	= REINTERPRET_CAST(const WaveFormatBlock*, buffer, offset);
	offset							+= WaveFormatBlockSize;

	const WaveDataBlock* data = NullPointer;
	while ( offset + WaveDataBlockSize <= byteCount )
	{
		data = REINTERPRET_CAST(const WaveDataBlock*, buffer, offset);
		if ( !FMemory.Equal( data->ID, "data", 4 ) )
		{
			offset += WaveDataBlockSize + data->size;
			continue;
		}

		break;
	}

	Wave ret = FWave.Construct( data->size, format->formatTag, format->channelCount, format->samplesPerSecond, format->bitsPerSample );
	FMemory.Copy(
		REINTERPRET_CAST(const Byte*, data, WaveDataBlockSize),
		ret.buffer,
		data->size
	);

	return ret;
}

static HeapMemory Encode(const Wave* instance)
{
	SizeType byteCount	= WaveDefaultAudioTrackOffset + instance->byteCount;
	HeapMemory ret		= FHeapMemory.Construct(byteCount);

	Void* buffer = ret.buffer;
	FMemory.Copy(
		instance->buffer,
		REINTERPRET_CAST(Byte*, buffer, WaveDefaultAudioTrackOffset),
		instance->byteCount
	);

	// set WaveRIFFHeader
	{
		WaveRIFFHeader* RIFF	= CAST(WaveRIFFHeader*, buffer);
		RIFF->size				= byteCount - 8;

		FMemory.Copy( "RIFF", RIFF->ID,		4 );
		FMemory.Copy( "WAVE", RIFF->format, 4 );
	}

	// set WaveFormatBlock
	{
		WaveFormatBlock* format			= REINTERPRET_CAST(WaveFormatBlock*, buffer, WaveRIFFHeaderSize);
		format->size					= WaveFormatBlockSize - 8;
		format->formatTag				= instance->formatTag;
		format->channelCount			= instance->channelCount;
		format->samplesPerSecond		= instance->samplesPerSecond;
		format->averageBytesPerSecond	= instance->audioFrameSize * instance->samplesPerSecond;
		format->blockAlign				= instance->audioFrameSize;
		format->bitsPerSample			= instance->bitsPerSample;

		FMemory.Copy( "fmt ", format->ID, 4 );
	}

	// set WaveDataBlock
	{
		WaveDataBlock* data	= REINTERPRET_CAST( WaveDataBlock*, buffer, WaveRIFFHeaderSize + WaveFormatBlockSize );
		data->size			= instance->byteCount;

		FMemory.Copy( "data", data->ID, 4 );
	}

	return ret;
}


struct FWave FWave = 
{
	Construct,
	Destruct,
	SetAudioFrame,
	GetAudioFrame,
	IsValid,
	Decode,
	Encode,
};