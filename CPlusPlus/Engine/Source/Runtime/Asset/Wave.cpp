#include "Wave.h"



namespace Engine
{
	Wave::Wave()
	{
		Memory::Clear( this, sizeof(Wave) );
	}

	Wave::Wave(SizeType preallocated, FormatTag formatTag, SizeType channelCount, SizeType samplesPerSecond, SizeType bitsPerSample)
	{
		this->buffer			= Allocator::Allocate<Byte>(preallocated);
		this->byteCount			= preallocated;
		this->formatTag			= formatTag;
		this->channelCount		= channelCount;
		this->samplesPerSecond	= samplesPerSecond;
		this->bitsPerSample		= bitsPerSample;
		this->audioFrameSize	= bitsPerSample / 8 * channelCount;
	}

	Wave::Wave(const Wave& other)
	{
		CopyFrom(other);
	}

	Wave::Wave(Wave&& other)
	{
		buffer				= other.buffer;
		byteCount			= other.byteCount;
		formatTag			= other.formatTag;
		channelCount		= other.channelCount;
		samplesPerSecond	= other.samplesPerSecond;
		bitsPerSample		= other.bitsPerSample;
		audioFrameSize		= other.audioFrameSize;

		other.buffer = nullptr;
	}

	Wave::~Wave()
	{
		Allocator::Release( reinterpret_cast<Byte*>(buffer) );
	}

	Void Wave::SetAudioFrame(SizeType index, const Void* audioFrame)
	{
		Memory::Copy(
			audioFrame,
			( *this )[index],
			audioFrameSize
		);
	}

	const Void* Wave::GetAudioFrame(SizeType index) const
	{
		return ( *this )[index];
	}

	Bool Wave::IsValid(const Void* buffer, SizeType byteCount)
	{
		if ( byteCount < DefaultAudioTrackOffset )
			return false;

		{
			auto descriptor = reinterpret_cast<const RIFFHeader*>(buffer);

			if (
				!Memory::Equal( descriptor->ID, "RIFF", 4 )
				)
				return false;

			if ( byteCount != descriptor->size + 8 )
				return false;

			if (
				!Memory::Equal( descriptor->format, "WAVE", 4 )
				)
				return false;
		}

		{
			auto descriptor = reinterpret_cast<const FormatBlock*>( reinterpret_cast<const Byte*>(buffer) + RIFFHeaderSize );

			if (
				!Memory::Equal( descriptor->ID, "fmt ", 4 )
				)
				return false;

			if (
				descriptor->formatTag != static_cast<EnumUnderlyingType<FormatTag>>( FormatTag::PCM			) &&
				descriptor->formatTag != static_cast<EnumUnderlyingType<FormatTag>>( FormatTag::IEEE_Float	)
				)
				return false;
		}

		return true;
	}

	Wave Wave::Decode(const Void* buffer, SizeType byteCount)
	{
		auto formatBlock = reinterpret_cast<const FormatBlock*>( reinterpret_cast<const Byte*>(buffer) + RIFFHeaderSize );

		const DataBlock* dataBlock	= nullptr;
		SizeType offset				= RIFFHeaderSize + ( 8 + formatBlock->size );
		while ( offset + DataBlockSize <= byteCount )
		{
			dataBlock = reinterpret_cast<const DataBlock*>( reinterpret_cast<const Byte*>(buffer) + offset );
			if (
				!Memory::Equal( dataBlock->ID, "data", 4 )
				)
			{
				offset += DataBlockSize + dataBlock->size;
				continue;
			}

			break;
		}

		Wave ret(
			dataBlock->size,
			static_cast<FormatTag>( formatBlock->formatTag ),
			formatBlock->channelCount,
			formatBlock->samplesPerSecond,
			formatBlock->bitsPerSample
		);
		Memory::Copy(
			dataBlock + 1,
			ret.GetBuffer(),
			dataBlock->size
		);

		return ret;
	}

	HeapMemory Wave::Encode(const Wave& instance)
	{
		SizeType totalSize = DefaultAudioTrackOffset + instance.byteCount;

		HeapMemory ret(totalSize);
		{
			auto buffer = ret.GetBuffer();
			Memory::Copy(
				instance.buffer,
				reinterpret_cast<Byte*>(buffer) + DefaultAudioTrackOffset,
				instance.byteCount
			);

			{
				auto descriptor		= reinterpret_cast<RIFFHeader*>(buffer);
				descriptor->size	= totalSize - 8;

				Memory::Copy( "RIFF", descriptor->ID,		4 );
				Memory::Copy( "WAVE", descriptor->format,	4 );
			}

			{
				auto descriptor						= reinterpret_cast<FormatBlock*>( reinterpret_cast<Byte*>(buffer) + RIFFHeaderSize );
				descriptor->size					= FormatBlockSize - 8;
				descriptor->formatTag				= static_cast<EnumUnderlyingType<FormatTag>>( instance.formatTag );
				descriptor->channelCount			= instance.channelCount;
				descriptor->samplesPerSecond		= instance.samplesPerSecond;
				descriptor->averageBytesPerSecond	= instance.audioFrameSize * instance.samplesPerSecond;
				descriptor->blockAlign				= instance.audioFrameSize;
				descriptor->bitsPerSample			= instance.bitsPerSample;
			}

			{
				auto descriptor		= reinterpret_cast<DataBlock*>( reinterpret_cast<Byte*>(buffer) + RIFFHeaderSize + FormatBlockSize );
				descriptor->size	= instance.byteCount;

				Memory::Copy( "data", descriptor->ID, 4 );
			}
		}

		return ret;
	}

	Void* Wave::GetBuffer()
	{
		return buffer;
	}

	const Void* Wave::GetBuffer() const
	{
		return buffer;
	}

	SizeType Wave::GetByteCount() const
	{
		return byteCount;
	}

	Wave::FormatTag Wave::GetFormatTag() const
	{
		return formatTag;
	}

	SizeType Wave::GetChannelCount() const
	{
		return channelCount;
	}

	SizeType Wave::GetSamplesPerSecond() const
	{
		return samplesPerSecond;
	}

	SizeType Wave::GetBitsPerSample() const
	{
		return bitsPerSample;
	}

	SizeType Wave::GetAudioFrameSize() const
	{
		return audioFrameSize;
	}

	Wave& Wave::operator=(const Wave& other)
	{
		this->~Wave();
		CopyFrom(other);

		return *this;
	}

	Wave& Wave::operator=(Wave&& other)
	{
		Swap( &buffer,				&other.buffer			);
		Swap( &byteCount,			&other.byteCount		);
		Swap( &formatTag,			&other.formatTag		);
		Swap( &channelCount,		&other.channelCount		);
		Swap( &samplesPerSecond,	&other.samplesPerSecond	);
		Swap( &bitsPerSample,		&other.bitsPerSample	);
		Swap( &audioFrameSize,		&other.audioFrameSize	);

		return *this;
	}

	Void* Wave::operator[](SizeType index)
	{
		return reinterpret_cast<Byte*>(buffer) + audioFrameSize * index;
	}

	const Void* Wave::operator[](SizeType index) const
	{
		return reinterpret_cast<const Byte*>(buffer) + audioFrameSize * index;
	}

	Void Wave::CopyFrom(const Wave& instance)
	{
		buffer				= Allocator::Allocate<Byte>( instance.byteCount );
		byteCount			= instance.byteCount;
		formatTag			= instance.formatTag;
		channelCount		= instance.channelCount;
		samplesPerSecond	= instance.samplesPerSecond;
		bitsPerSample		= instance.bitsPerSample;
		audioFrameSize		= instance.audioFrameSize;

		Memory::Copy(
			instance.buffer,
			buffer,
			instance.byteCount
		);
	}
}