#include "DirectSound.h"

#include "Windows.h"

#include <dsound.h>
#pragma comment(lib, "dsound")



namespace Engine::Windows
{
	DirectSound::DirectSound(Void* nativeWindowsWindowHandle, SizeType preallocatedSoundBufferSize, SizeType waveFormatTag, SizeType channelCount, SizeType samplesPerSecond, SizeType bitsPerSample)
	{
		if ( !nativeWindowsWindowHandle )
			nativeWindowsWindowHandle = GetDesktopWindow();

		IDirectSound* sound = nullptr;
		{
			DirectSoundCreate(
				nullptr,
				&sound,
				nullptr
			);

			sound->SetCooperativeLevel(
				reinterpret_cast<HWND>(nativeWindowsWindowHandle),
				DSSCL_NORMAL
			);
		}

		IDirectSoundBuffer* soundBuffer = nullptr;
		{
			const SizeType AudioFrameSize = bitsPerSample / 8 * channelCount;

			WAVEFORMATEX format;
			{
				Memory::Clear(
					&format,
					sizeof(WAVEFORMATEX)
				);

				format.wFormatTag		= waveFormatTag;
				format.nChannels		= channelCount;
				format.nSamplesPerSec	= samplesPerSecond;
				format.nAvgBytesPerSec	= AudioFrameSize * samplesPerSecond;
				format.nBlockAlign		= AudioFrameSize;
				format.wBitsPerSample	= bitsPerSample;
			}

			DSBUFFERDESC descriptor;
			{
				Memory::Clear(
					&descriptor,
					sizeof(DSBUFFERDESC)
				);

				descriptor.dwSize			= sizeof(DSBUFFERDESC);
				descriptor.dwFlags			= DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
				descriptor.dwBufferBytes	= preallocatedSoundBufferSize;
				descriptor.lpwfxFormat		= &format;
			}

			sound->CreateSoundBuffer(
				&descriptor,
				&soundBuffer,
				nullptr
			);
		}

		this->sound							= sound;
		this->soundBuffer					= soundBuffer;
		this->preallocatedSoundBufferSize	= preallocatedSoundBufferSize;
		this->writePosition					= 0;

		Play();
	}

	DirectSound::~DirectSound()
	{
		auto sound			= reinterpret_cast<IDirectSound*>( this->sound );
		auto soundBuffer	= reinterpret_cast<IDirectSoundBuffer*>( this->soundBuffer );

		if (sound)
			sound->Release();

		if (soundBuffer)
			soundBuffer->Release();
	}

	Void DirectSound::Fill(const Void* buffer, SizeType byteCount)
	{
		if ( byteCount > preallocatedSoundBufferSize )
			return;

		auto soundBuffer = reinterpret_cast<IDirectSoundBuffer*>( this->soundBuffer );

		DWORD writableBufferSize;

		{
			writableBufferSize = preallocatedSoundBufferSize - writePosition;
			if ( writableBufferSize > byteCount )
				writableBufferSize = byteCount;

			{
				Void* lockedBuffer;
				DWORD lockedBufferSize;

				soundBuffer->Lock(
					writePosition,
					writableBufferSize,
					&lockedBuffer,
					&lockedBufferSize,
					nullptr,
					nullptr,
					0
				);

				Memory::Copy(buffer, lockedBuffer, lockedBufferSize);

				soundBuffer->Unlock(
					lockedBuffer,
					lockedBufferSize,
					nullptr,
					0
				);
			}

			writePosition += writableBufferSize;
			if ( writePosition == preallocatedSoundBufferSize )
				writePosition = 0;

			byteCount -= writableBufferSize;
		}

		if ( byteCount != 0 )
		{
			buffer				= reinterpret_cast<const Byte*>(buffer) + writableBufferSize;
			writableBufferSize	= byteCount;

			{
				Void* lockedBuffer;
				DWORD lockedBufferSize;

				soundBuffer->Lock(
					writePosition,
					writableBufferSize,
					&lockedBuffer,
					&lockedBufferSize,
					nullptr,
					nullptr,
					0
				);

				Memory::Copy(buffer, lockedBuffer, lockedBufferSize);

				soundBuffer->Unlock(
					lockedBuffer,
					lockedBufferSize,
					nullptr,
					0
				);
			}

			writePosition += writableBufferSize;
		}
	}

	Void DirectSound::Play()
	{
		reinterpret_cast<IDirectSoundBuffer*>(soundBuffer)->Play( 0, 0, DSBPLAY_LOOPING );
	}

	Void DirectSound::Stop()
	{
		reinterpret_cast<IDirectSoundBuffer*>(soundBuffer)->Stop();
	}

	Void DirectSound::SetVolumn(RealType volumn)
	{
		// todo..
	}

	SizeType DirectSound::CalculatedWritableBufferSize() const
	{
		auto soundBuffer = reinterpret_cast<IDirectSoundBuffer*>( this->soundBuffer );

		DWORD readCursor;
		DWORD writeCursor;
		soundBuffer->GetCurrentPosition(
			&readCursor,
			&writeCursor
		);

		if ( writePosition > readCursor )
			return preallocatedSoundBufferSize - ( writePosition - readCursor );
	
		return readCursor - writePosition;
	}
}