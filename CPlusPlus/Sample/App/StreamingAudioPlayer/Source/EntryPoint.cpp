#include "Engine.h"



using namespace Engine;
using namespace Engine::Windows;


Int32 EntryPoint()
{
	U8String filename;
	{
		Char16 buffer[256];
		if ( !FileDialog::Open( buffer, sizeof(buffer) / sizeof(Char16), L"(*.wav)\0*.wav\0" ) )
			return -1;

		filename = U16String(buffer);
	}

	Wave wave;
	{
		auto loaded = File::ReadAllBytes(filename);

		wave = Wave::Decode( loaded.GetBuffer(), loaded.GetByteCount() );
	}

	const SizeType ASecond				= wave.GetAudioFrameSize() * wave.GetSamplesPerSecond();
	const SizeType SoundBufferSize		= ASecond / 4;
	const SizeType HalfSoundBufferSize	= SoundBufferSize / 2;

	DirectSound directSound( nullptr, SoundBufferSize, static_cast<SizeType>(wave.GetFormatTag()), wave.GetChannelCount(), wave.GetSamplesPerSecond(), wave.GetBitsPerSample() );

	SizeType offset = 0;
	while (true)
	{
		SizeType writableBufferSize = directSound.CalculatedWritableBufferSize();
		if ( writableBufferSize > HalfSoundBufferSize )
		{
			SizeType readableBufferSize = wave.GetByteCount() - offset;
			if ( writableBufferSize > readableBufferSize )
				writableBufferSize = readableBufferSize;

			directSound.Fill( reinterpret_cast<const Byte*>( wave.GetBuffer() ) + offset, writableBufferSize );

			offset += writableBufferSize;
			if ( offset == wave.GetByteCount() )
				offset = 0;
		}
	}

	return 0;
}


int main(int count, char** parameters)
{
	return EntryPoint();
}