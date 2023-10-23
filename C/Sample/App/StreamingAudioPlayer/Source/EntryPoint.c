/**
* Playing A *.wav File By DSound(Windows OS Component)
*/

#include "Engine.h"



Int32 EntryPoint()
{
	U8String filename;
	{
		Char16 buffer[256];
		if (!FFileDialog.Open(buffer, sizeof(buffer) / sizeof(Char16), L"(*.wav)\0*.wav\0", L"./", L"Open *.wav", NullPointer))
			return -1;

		U16String u16 = FU16String.Construct(buffer);

		filename = FU16String.ToU8String(&u16);

		FU16String.Destruct(&u16);
	}

	Wave wave;
	{
		HeapMemory loaded = FFile.ReadAllBytes(&filename);

		wave = FWave.Decode(loaded.buffer, loaded.byteCount);

		FHeapMemory.Destruct(&loaded);
	}

	const SizeType ASecond = wave.audioFrameSize * wave.samplesPerSecond;
	const SizeType SoundBufferSize = ASecond / 4;
	const SizeType HalfSoundBufferSize = SoundBufferSize / 2;

	DirectSound directSound = FDirectSound.Construct(NullPointer, SoundBufferSize, wave.formatTag, wave.channelCount, wave.bitsPerSample, wave.samplesPerSecond);

	SizeType offset = 0;
	while (True)
	{
		SizeType writableBufferSize = FDirectSound.CalculateWritableBufferSize(&directSound);
		if (writableBufferSize > HalfSoundBufferSize )
		{
			SizeType readableBufferSize = wave.byteCount - offset;
			if (writableBufferSize > readableBufferSize)
				writableBufferSize = readableBufferSize;

			FDirectSound.Fill(&directSound, CAST(Byte*, wave.buffer) + offset, writableBufferSize);

			offset += writableBufferSize;
			if (offset == wave.byteCount)
				offset = 0;
		}
	}

	FDirectSound.Destruct(&directSound);

	FWave.Destruct(&wave);
	FU8String.Destruct(&filename);

	return 0;
}


int main(int count, char** arguments)
{
	return EntryPoint();
}