#include "DirectSound.h"

#include "Windows.h"

#include <dsound.h>
#pragma comment(lib, "dsound")



static DirectSound Construct(Void* nativeWindowsWindowHandle, SizeType preallocatedSoundBufferSize, SizeType waveFormatTag, SizeType channelCount, SizeType depth, SizeType samplesPerSecond)
{
	if ( !nativeWindowsWindowHandle )
		nativeWindowsWindowHandle = GetDesktopWindow();

	IDirectSound* sound = NullPointer;
	{
		DirectSoundCreate( NullPointer, &sound, NullPointer );
		sound->lpVtbl->SetCooperativeLevel( sound, CAST(HWND, nativeWindowsWindowHandle), DSSCL_NORMAL );
	}

	IDirectSoundBuffer* soundBuffer = NullPointer;
	{
		const SizeType AudioFrameSize = depth / 8 * channelCount;

		WAVEFORMATEX format;
		format.wFormatTag		= waveFormatTag;
		format.nChannels		= channelCount;
		format.nSamplesPerSec	= samplesPerSecond;
		format.nAvgBytesPerSec	= AudioFrameSize * samplesPerSecond;
		format.nBlockAlign		= AudioFrameSize;
		format.wBitsPerSample	= depth;
		format.cbSize			= 0;

		DSBUFFERDESC descriptor;
		FMemory.Clear( &descriptor, sizeof(DSBUFFERDESC) );
		descriptor.dwSize			= sizeof(DSBUFFERDESC);
		descriptor.dwFlags			= DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
		descriptor.dwBufferBytes	= preallocatedSoundBufferSize;
		descriptor.lpwfxFormat		= &format;

		sound->lpVtbl->CreateSoundBuffer( sound, &descriptor, &soundBuffer, NullPointer );
	}

	DirectSound ret;
	ret.preallocatedSoundBufferSize	= preallocatedSoundBufferSize;
	ret.sound						= sound;
	ret.soundBuffer					= soundBuffer;
	ret.writePosition				= 0;

	FDirectSound.Play( &ret );

	return ret;
}

static Void Destruct(DirectSound* instance)
{
	IDirectSound* sound				= CAST( IDirectSound*, instance->sound );
	IDirectSoundBuffer* soundBuffer	= CAST( IDirectSoundBuffer*, instance->soundBuffer );

	if (sound)
		sound->lpVtbl->Release(sound);

	if (soundBuffer)
		soundBuffer->lpVtbl->Release(soundBuffer);
}

static Void Fill(DirectSound* instance, const Void* buffer, SizeType byteCount)
{
	if ( byteCount > instance->preallocatedSoundBufferSize )
		return;

	IDirectSoundBuffer* soundBuffer = CAST( IDirectSoundBuffer*, instance->soundBuffer );

	UInt32 writableBufferSize;

	{
		writableBufferSize = instance->preallocatedSoundBufferSize - instance->writePosition;
		if ( writableBufferSize > byteCount )
			writableBufferSize = byteCount;

		Void* lockedBuffer;
		UInt32 lockedSize;
		soundBuffer->lpVtbl->Lock( soundBuffer, instance->writePosition, writableBufferSize, &lockedBuffer, &lockedSize, NullPointer, NullPointer, 0 );
		FMemory.Copy(buffer, lockedBuffer, lockedSize);
		soundBuffer->lpVtbl->Unlock(soundBuffer, lockedBuffer, lockedSize, NullPointer, NullPointer);

		instance->writePosition += writableBufferSize;
		if ( instance->writePosition == instance->preallocatedSoundBufferSize )
			instance->writePosition = 0;
		
		byteCount -= writableBufferSize;
	}

	if ( byteCount != 0 )
	{
		buffer				= CAST(const Byte*, buffer) + writableBufferSize;
		writableBufferSize	= byteCount;

		Void* lockedBuffer;
		UInt32 lockedSize;
		soundBuffer->lpVtbl->Lock( soundBuffer, instance->writePosition, writableBufferSize, &lockedBuffer, &lockedSize, NullPointer, NullPointer, 0 );
		FMemory.Copy(buffer, lockedBuffer, lockedSize);
		soundBuffer->lpVtbl->Unlock(soundBuffer, lockedBuffer, lockedSize, NullPointer, NullPointer);

		instance->writePosition += writableBufferSize;
	}
}

static Void Play(DirectSound* instance)
{
	IDirectSoundBuffer* soundBuffer = CAST( IDirectSoundBuffer*, instance->soundBuffer );

	soundBuffer->lpVtbl->Play(soundBuffer, 0, 0, DSBPLAY_LOOPING);
}

static Void Stop(DirectSound* instance)
{
	IDirectSoundBuffer* soundBuffer = CAST( IDirectSoundBuffer*, instance->soundBuffer );

	soundBuffer->lpVtbl->Stop(soundBuffer);
}

static Void SetVolumn(DirectSound* instance, RealType volumn)
{
	// todo..
}

static SizeType CalculateWritableBufferSize(const DirectSound* instance)
{
	IDirectSoundBuffer* soundBuffer = CAST( IDirectSoundBuffer*, instance->soundBuffer );

	UInt32 readCursor;
	UInt32 writeCursor;
	soundBuffer->lpVtbl->GetCurrentPosition( soundBuffer, &readCursor, &writeCursor );

	if ( instance->writePosition > readCursor )
		return instance->preallocatedSoundBufferSize - ( instance->writePosition - readCursor );
	else
		return readCursor - instance->writePosition;
}


struct FDirectSound FDirectSound = 
{
	Construct,
	Destruct,
	Fill,
	Play,
	Stop,
	SetVolumn,
	CalculateWritableBufferSize,
};
