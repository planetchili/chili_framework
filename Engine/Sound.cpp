/****************************************************************************************** 
 *	Chili DirectX Framework Sound Pack Version 16.11.11									  *	
 *	Sound.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with this source code.  If not, see <http://www.gnu.org/licenses/>.			  *
 ******************************************************************************************/
#include "Sound.h"
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <array>
#include <functional>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <Propvarutil.h>
#include "XAudio\XAudio2.h"
#include "DXErr.h"

#pragma comment( lib,"mfplat.lib" )
#pragma comment( lib,"mfreadwrite.lib" )
#pragma comment( lib,"mfuuid.lib" )
#pragma comment( lib,"Propsys.lib" )

#define CHILI_SOUND_API_EXCEPTION( hr,note ) SoundSystem::APIException( hr,_CRT_WIDE(__FILE__),__LINE__,note )
#define CHILI_SOUND_FILE_EXCEPTION( filename,note ) SoundSystem::FileException( _CRT_WIDE(__FILE__),__LINE__,note,filename )

SoundSystem& SoundSystem::Get()
{
	static SoundSystem instance;
	return instance;
}

 void SoundSystem::SetMasterVolume( float vol )
 {
	 HRESULT hr;
	 if( FAILED( hr = Get().pMaster->SetVolume( vol ) ) )
	 {
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Setting master volume" );
	 }
 }

const WAVEFORMATEX& SoundSystem::GetFormat()
{
	return *Get().format;
}

void SoundSystem::PlaySoundBuffer( Sound & s,float freqMod,float vol )
{
	std::lock_guard<std::mutex> lock( mutex );
	if( idleChannelPtrs.size() > 0 )
	{
		activeChannelPtrs.push_back( std::move( idleChannelPtrs.back() ) );
		idleChannelPtrs.pop_back();
		activeChannelPtrs.back()->PlaySoundBuffer( s,freqMod,vol );
	}
}

SoundSystem::XAudioDll::XAudioDll()
{
	LoadType type = LoadType::System;
	while( true )
	{
		hModule = LoadLibrary( GetDllPath( type ) );
		if( hModule != 0 )
		{
			return;
		}
		else
		{
			switch( type )
			{
			case LoadType::System:
				type = LoadType::Folder;
				break;
			case LoadType::Folder:
				type = LoadType::Local;
				break;
			case LoadType::Local:
				throw CHILI_SOUND_API_EXCEPTION(
					HRESULT_FROM_WIN32( GetLastError() ),
					std::wstring(
						L"The XAudio2 DLL Could not be loaded. It is required that:\n"
						L"A) [ " ) + std::wstring( GetDllPath( LoadType::Folder ) ) +
					std::wstring( L" ] exist in the same folder as this executable;\n"
						L"B) [ " ) + std::wstring( GetDllPath( LoadType::Local ) ) +
					std::wstring( L" ] exist in the same folder as this executable; or\n"
						L"C) [ XAudio2_7.dll ] be installed on this system via the DirectX"
						L" Redistributable Installer Version June 2010\n" ) );
			default:
				assert( false && "Bad LoadType encountered in XAudio Dll loading sequence loop" );
			}
		}
	}
}

SoundSystem::XAudioDll::~XAudioDll()
{
	if( hModule != 0 )
	{
		FreeLibrary( hModule );
		hModule = 0;
	}
}

SoundSystem::XAudioDll::operator HMODULE() const
{
	return hModule;
}

const wchar_t* SoundSystem::XAudioDll::GetDllPath( LoadType type )
{
	switch( type )
	{
	case LoadType::System:
		return systemPath;
	case LoadType::Folder:
		return folderPath;
	case LoadType::Local:
		return localPath;
	default:
		assert( false && "Bad LoadType in GetDllPath function" );
		return nullptr;
	}
}

SoundSystem::SoundSystem()
	:
	format( std::make_unique<WAVEFORMATEX>() )
{
	// setup wave format info structure
	static_assert(nChannelsPerSound > 0u,"WAVE File Format Error: At least one channel required per audio stream");
	static_assert(nChannelsPerSound <= XAUDIO2_MAX_AUDIO_CHANNELS,"WAVE File Format Error: Maximum audio channels per audio stream exceeded");
	static_assert(nSamplesPerSec >= XAUDIO2_MIN_SAMPLE_RATE,"WAVE File Format Error: Sample rate lower than minimum allowed");
	static_assert(nSamplesPerSec <= XAUDIO2_MAX_SAMPLE_RATE,"WAVE File Format Error: Sample rate exceeds maximum allowed");
	static_assert(nBitsPerSample > 0u,"WAVE File Format Error: Bit depth of 0 bits per sample is not allowed");
	static_assert(nBitsPerSample % 8u == 0,"WAVE File Format Error: Bit depth must be multiple of 8");
	format->nChannels = nChannelsPerSound;
	format->nSamplesPerSec = nSamplesPerSec;
	format->wBitsPerSample = nBitsPerSample;
	format->nBlockAlign = (nBitsPerSample / 8) * nChannelsPerSound;
	format->nAvgBytesPerSec = format->nBlockAlign * nSamplesPerSec;
	format->cbSize = 0;
	format->wFormatTag = WAVE_FORMAT_PCM;
	
	// find address of DllGetClassObject() function in the dll
	const std::function<HRESULT(REFCLSID,REFIID,LPVOID)> DllGetClassObject =
        reinterpret_cast<HRESULT(WINAPI*)(REFCLSID,REFIID,LPVOID)>( 
		GetProcAddress( xaudio_dll,"DllGetClassObject" ) );
	if( !DllGetClassObject )
	{		
		throw CHILI_SOUND_API_EXCEPTION( 
			HRESULT_FROM_WIN32( GetLastError() ),
			L"Getting process address of 'DllGetClassObject' function" );
	}

	// create the factory class for the XAudio2 component object
	Microsoft::WRL::ComPtr<IClassFactory> pClassFactory;
	HRESULT hr;
	if( FAILED( hr = DllGetClassObject( 
		 __uuidof( XAudio2 ),
		IID_IClassFactory,
		pClassFactory.ReleaseAndGetAddressOf() ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Creating factory for XAudio2 object" );
	}

	// create the XAudio2 component object itself
	if( FAILED( hr = pClassFactory->CreateInstance( nullptr,
		__uuidof( IXAudio2 ),&pEngine ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Creating XAudio2 object" );
	}

	// initialize the XAudio2 component object
	if( FAILED( hr = pEngine->Initialize( 0,XAUDIO2_DEFAULT_PROCESSOR ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Initializing XAudio2 object" );
	}

	// create the mastering voice
	if( FAILED( hr = pEngine->CreateMasteringVoice( &pMaster ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Creating mastering voice" );
	}

	// create channel objects
	for( int i = 0; i < nChannels; i++ )
	{
		idleChannelPtrs.push_back( std::make_unique<Channel>( *this ) );
	}
}

void SoundSystem::DeactivateChannel( Channel & channel )
{
	std::lock_guard<std::mutex> lock( mutex );
	auto i = std::find_if( activeChannelPtrs.begin(),activeChannelPtrs.end(),
		[&channel]( const std::unique_ptr<Channel>& pChan ) -> bool
	{
		return &channel == pChan.get();
	} );
	idleChannelPtrs.push_back( std::move( *i ) );
	activeChannelPtrs.erase( i );
}

SoundSystem::Channel::Channel( SoundSystem & sys )
	:
	xaBuffer( std::make_unique<XAUDIO2_BUFFER>() )
{
	class VoiceCallback : public IXAudio2VoiceCallback
	{
	public:
		void STDMETHODCALLTYPE OnStreamEnd() override
		{}
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override
		{}
		void STDMETHODCALLTYPE OnVoiceProcessingPassStart( UINT32 SamplesRequired ) override
		{}
		void STDMETHODCALLTYPE OnBufferEnd( void* pBufferContext ) override
		{
			Channel& chan = *reinterpret_cast<Channel*>( pBufferContext );
			chan.Stop();
			{
				std::lock_guard<std::mutex> lock( chan.pSound->mutex );
				chan.pSound->activeChannelPtrs.erase( std::find(
					chan.pSound->activeChannelPtrs.begin(),chan.pSound->activeChannelPtrs.end(),&chan ) );
				// notify any thread that might be waiting for activeChannels
				// to become zero (i.e. thread calling destructor)
				chan.pSound->cvDeath.notify_all();
			}
			chan.pSound = nullptr;
			SoundSystem::Get().DeactivateChannel( chan );
		}
		void STDMETHODCALLTYPE OnBufferStart( void* pBufferContext ) override
		{}
		void STDMETHODCALLTYPE OnLoopEnd( void* pBufferContext ) override
		{}
		void STDMETHODCALLTYPE OnVoiceError( void* pBufferContext,HRESULT Error ) override
		{}
	};
	static VoiceCallback vcb;
	ZeroMemory( xaBuffer.get(),sizeof( *xaBuffer ) );
	xaBuffer->pContext = this;
	HRESULT hr;
	if( FAILED( hr = sys.pEngine->CreateSourceVoice( &pSource,sys.format.get(),0u,2.0f,&vcb ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Creating source voice for channel" );
	}
}

SoundSystem::Channel::~Channel()
{
	assert( !pSound );
	if( pSource )
	{
		pSource->DestroyVoice();
		pSource = nullptr;
	}
}

void SoundSystem::Channel::PlaySoundBuffer( Sound& s,float freqMod,float vol )
{
	assert( pSource && !pSound );
	{
		std::lock_guard<std::mutex> lock( s.mutex );
		s.activeChannelPtrs.push_back( this );
	}
	// callback thread not running yet, so no sync necessary for pSound
	pSound = &s;
	xaBuffer->pAudioData = s.pData.get();
	xaBuffer->AudioBytes = s.nBytes;
	if( s.looping )
	{
		xaBuffer->LoopBegin = s.loopStart;
		xaBuffer->LoopLength = s.loopEnd - s.loopStart;
		xaBuffer->LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		xaBuffer->LoopCount = 0u;
	}
	HRESULT hr;
	if( FAILED( hr = pSource->SubmitSourceBuffer( xaBuffer.get(),nullptr ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Starting playback - submitting source buffer" );
	}
	if( FAILED( hr = pSource->SetFrequencyRatio( freqMod ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Starting playback - setting frequency" );
	}
	if( FAILED( hr = pSource->SetVolume( vol ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Starting playback - setting volume" );
	}
	if( FAILED( hr = pSource->Start() ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Starting playback - starting" );
	}
}

void SoundSystem::Channel::Stop()
{
	assert( pSource && pSound );
	pSource->Stop();
	pSource->FlushSourceBuffers();
}

void SoundSystem::Channel::RetargetSound( const Sound* pOld,Sound* pNew )
{
	assert( pOld == pSound );
	pSound = pNew;
}

Sound::Sound( const std::wstring& fileName,bool loopingWithAutoCueDetect )
	:
	Sound( fileName,loopingWithAutoCueDetect ? 
		LoopType::AutoEmbeddedCuePoints : LoopType::NotLooping )
{
}

Sound::Sound( const std::wstring& fileName,LoopType loopType )
{
	if( fileName.substr( fileName.size() - 4u,4u ) == std::wstring{ L".wav" } )
	{
		*this = Sound( fileName,loopType,nullSample,nullSample,nullSeconds,nullSeconds );
	}
	else
	{
		*this = LoadNonWav( fileName,loopType,nullSample,nullSample,nullSeconds,nullSeconds );
	}
}

Sound::Sound( const std::wstring& fileName,unsigned int loopStart,unsigned int loopEnd )
{
	if( fileName.substr( fileName.size() - 4u,4u ) == std::wstring{ L".wav" } )
	{
		*this = Sound( fileName,LoopType::ManualSample,loopStart,loopEnd,nullSeconds,nullSeconds );
	}
	else
	{
		*this = LoadNonWav( fileName,LoopType::ManualSample,loopStart,loopEnd,nullSeconds,nullSeconds );
	}
}

Sound::Sound( const std::wstring& fileName,float loopStart,float loopEnd )
{
	if( fileName.substr( fileName.size() - 4u,4u ) == std::wstring{ L".wav" } )
	{
		*this = Sound( fileName,LoopType::ManualFloat,nullSample,nullSample,loopStart,loopEnd );
	}
	else
	{
		*this = LoadNonWav( fileName,LoopType::ManualFloat,nullSample,nullSample,loopStart,loopEnd );
	}
}

Sound Sound::LoadNonWav( const std::wstring& fileName,LoopType loopType,
						 unsigned int loopStartSample,unsigned int loopEndSample,
						 float loopStartSeconds,float loopEndSeconds )
{
	namespace wrl = Microsoft::WRL;

	// if manual float looping, second inputs cannot be null
	assert( (loopType == LoopType::ManualFloat) !=
		(loopStartSeconds == nullSeconds || loopEndSeconds == nullSeconds) &&
			"Did you pass a LoopType::Manual to the constructor? (BAD!)" );
	// if manual sample looping, sample inputs cannot be null
	assert( (loopType == LoopType::ManualSample) !=
		(loopStartSample == nullSample || loopEndSample == nullSample) &&
			"Did you pass a LoopType::Manual to the constructor? (BAD!)" );
	// load from non-wav cannot use embedded loop points
	assert( loopType != LoopType::AutoEmbeddedCuePoints &&
			"load from non-wav cannot use embedded loop points" );

	Sound sound;
	HRESULT hr;

	// make sure that the sound system is loaded first!
	SoundSystem::Get();

	// creating source reader
	wrl::ComPtr<IMFSourceReader> pReader;
	if( FAILED( hr = MFCreateSourceReaderFromURL( fileName.c_str(),nullptr,&pReader ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"Creating MF Source Reader\nFilename: " + fileName );
	}

	// selecting first stream
	if( FAILED( hr = pReader->SetStreamSelection(
		(DWORD)MF_SOURCE_READER_ALL_STREAMS,FALSE ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"setting stream selection all" );
	}

	if( FAILED( hr = pReader->SetStreamSelection(
		(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,TRUE ) ) )
	{
		throw CHILI_SOUND_API_EXCEPTION( hr,L"setting stream selection first" );
	}


	// Configuring output format
	wrl::ComPtr<IMFMediaType> pUncompressedAudioType;
	{
		wrl::ComPtr<IMFMediaType> pPartialType;

		// configuring partial type
		if( FAILED( hr = MFCreateMediaType( &pPartialType ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"creating partial media type" );
		}

		if( FAILED( hr = pPartialType->SetGUID( MF_MT_MAJOR_TYPE,MFMediaType_Audio ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"setting partial media type major guid" );
		}

		if( FAILED( hr = pPartialType->SetGUID( MF_MT_SUBTYPE,MFAudioFormat_PCM ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"setting partial media type sub guid" );
		}

		// Set this type on the source reader. The source reader will
		// load the necessary decoder.
		if( FAILED( hr = pReader->SetCurrentMediaType(
				(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				nullptr,pPartialType.Get() ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"setting partial type on source reader" );
		}

		// Get the complete uncompressed format.
		if( FAILED( hr = pReader->GetCurrentMediaType(
				(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				&pUncompressedAudioType ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"getting complete uncompressed format" );
		}

		// Ensure the stream is selected.
		if( FAILED( hr = pReader->SetStreamSelection(
				(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				TRUE ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"making sure stream is selected (who the fuck knows?)" );
		}
	}

	// calculating number of sample bytes
	// and verifying that format matches sound system channels
	// and setting loop parameters
	{
		UINT32 cbFormat = 0;

		// init smart pointer with custom deleter
		const auto pFormat = [&]()
		{
			// to be temp filled with the pointer
			WAVEFORMATEX *pFormat = nullptr;
			// loading format info into wave format structure (callee allocated, but we must free)
			if( FAILED( hr = MFCreateWaveFormatExFromMFMediaType( pUncompressedAudioType.Get(),&pFormat,&cbFormat ) ) )
			{
				throw CHILI_SOUND_API_EXCEPTION( hr,L"loading format info into wave format structure" );
			}
			return std::unique_ptr<WAVEFORMATEX,decltype(&CoTaskMemFree)>( pFormat,CoTaskMemFree );
		}();

		// compare format with sound system format
		{
			const WAVEFORMATEX& sysFormat = SoundSystem::GetFormat();

			if( pFormat->nChannels != sysFormat.nChannels )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad decompressed wave format (nChannels)" );
			}
			else if( pFormat->wBitsPerSample != sysFormat.wBitsPerSample )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad decompressed wave format (wBitsPerSample)" );
			}
			else if( pFormat->nSamplesPerSec != sysFormat.nSamplesPerSec )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad decompressed wave format (nSamplesPerSec)" );
			}
			else if( pFormat->wFormatTag != sysFormat.wFormatTag )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad decompressed wave format (wFormatTag)" );
			}
			else if( pFormat->nBlockAlign != sysFormat.nBlockAlign )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad decompressed wave format (nBlockAlign)" );
			}
			else if( pFormat->nAvgBytesPerSec != sysFormat.nAvgBytesPerSec )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad decompressed wave format (nAvgBytesPerSec)" );
			}
		}

		{
			// inheritance for automatic freeing of propvariant resources
			struct AutoPropVariant : PROPVARIANT
			{
				~AutoPropVariant()
				{
					PropVariantClear( this );
				}
			} var;

			// get duration attribute (as prop variant) from reader
			if( FAILED( hr = pReader->GetPresentationAttribute( MF_SOURCE_READER_MEDIASOURCE,
				MF_PD_DURATION,&var ) ) )
			{
				throw CHILI_SOUND_API_EXCEPTION( hr,L"getting duration attribute from reader" );
			}

			// getting int64 from duration prop variant
			long long duration;
			if( FAILED( hr = PropVariantToInt64( var,&duration ) ) )
			{
				throw CHILI_SOUND_API_EXCEPTION( hr,L"getting int64 out of variant property (duration)" );
			}

			// calculating number of bytes for samples (duration is in units of 100ns)
			// (adding extra 1 sec of padding for length calculation error margin)
			sound.nBytes = UINT32( (pFormat->nAvgBytesPerSec * duration) / 10000000 + pFormat->nAvgBytesPerSec );
		}

		/////////////////////////////
		// setting looping parameters
		switch( loopType )
		{		
		case LoopType::ManualFloat:
		{
			sound.looping = true;

			const WAVEFORMATEX& sysFormat = SoundSystem::GetFormat();
			const unsigned int nFrames = sound.nBytes / sysFormat.nBlockAlign;

			const unsigned int nFramesPerSec = sysFormat.nAvgBytesPerSec / sysFormat.nBlockAlign;
			sound.loopStart = unsigned int( loopStartSeconds * float( nFramesPerSec ) );
			assert( sound.loopStart < nFrames );
			sound.loopEnd = unsigned int( loopEndSeconds * float( nFramesPerSec ) );
			assert( sound.loopEnd > sound.loopStart && sound.loopEnd < nFrames );

			// just in case ;)
			sound.loopStart = std::min( sound.loopStart,nFrames - 1u );
			sound.loopEnd = std::min( sound.loopEnd,nFrames - 1u );
		}
			break;
		case LoopType::ManualSample:
		{
			sound.looping = true;

			const WAVEFORMATEX& sysFormat = SoundSystem::GetFormat();
			const unsigned int nFrames = sound.nBytes / sysFormat.nBlockAlign;

			assert( loopStartSample < nFrames );
			sound.loopStart = loopStartSample;
			assert( loopEndSample > loopStartSample && loopEndSample < nFrames );
			sound.loopEnd = loopEndSample;

			// just in case ;)
			sound.loopStart = std::min( sound.loopStart,nFrames - 1u );
			sound.loopEnd = std::min( sound.loopEnd,nFrames - 1u );
		}
			break;
		case LoopType::AutoFullSound:
		{
			sound.looping = true;

			const unsigned int nFrames = sound.nBytes / SoundSystem::GetFormat().nBlockAlign;
			assert( nFrames != 0u && "Cannot auto full-loop on zero-length sound!" );
			sound.loopStart = 0u;
			sound.loopEnd = nFrames != 0u ? nFrames - 1u : 0u;
		}
			break;
		case LoopType::NotLooping:
			break;
		default:
			assert( "Bad LoopType encountered!" && false );
			break;
		}
		/////////////////////////////
	}
	
	// allocate memory for sample data
	sound.pData = std::make_unique<BYTE[]>( sound.nBytes );

	// decode samples and copy into data buffer
	size_t nBytesWritten = 0u;
	while( true )
	{
		wrl::ComPtr<IMFSample> pSample;
		DWORD dwFlags = 0;

		// Read the next samples
		if( FAILED( hr = pReader->ReadSample(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			0,nullptr,&dwFlags,nullptr,&pSample ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"reading next samples" );
		}

		if( dwFlags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED )
		{
			throw std::runtime_error( "Type change - not supported by WAVE file format.@ ReadSample" );
		}

		if( dwFlags & MF_SOURCE_READERF_ENDOFSTREAM )
		{
			break;
		}

		if( pSample == nullptr )
		{
			continue;
		}

		// Get a pointer to the audio data in the sample.
		wrl::ComPtr<IMFMediaBuffer> pBuffer;
		if( FAILED( hr = pSample->ConvertToContiguousBuffer( &pBuffer ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"converting to contiguous buffer" );
		}

		BYTE *pAudioData = nullptr;
		DWORD cbBuffer = 0;
		if( FAILED( hr = pBuffer->Lock( &pAudioData,nullptr,&cbBuffer ) ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"locking sample buffer" );
		}

		// Make sure not to exceed the size of the buffer
		if( nBytesWritten + cbBuffer > sound.nBytes )
		{
			throw std::runtime_error( "too many bytes being decoded wtf??!~" );
		}

		// copy the sample bytes
		memcpy( &sound.pData[nBytesWritten],pAudioData,cbBuffer );

		// Update running total of audio data.
		nBytesWritten += cbBuffer;
		
		// Unlock the buffer.
		if( FAILED( hr = pBuffer->Unlock() ) )
		{
			throw CHILI_SOUND_API_EXCEPTION( hr,L"unlocking sample buffer" );
		}
	}

	// reallocate buffer for proper size
	{
		auto pAdjustedBuffer = std::make_unique<BYTE[]>( nBytesWritten );
		// copy over bytes
		memcpy( pAdjustedBuffer.get(),sound.pData.get(),nBytesWritten );
		// move buffer
		sound.pData = std::move( pAdjustedBuffer );
		// adjust byte count
		sound.nBytes = UINT32( nBytesWritten );
	}	

	return std::move( sound );
}

Sound::Sound( const std::wstring& fileName,LoopType loopType,
	unsigned int loopStartSample,unsigned int loopEndSample,
	float loopStartSeconds,float loopEndSeconds )
{
	// if manual float looping, second inputs cannot be null
	assert( (loopType == LoopType::ManualFloat) !=
		(loopStartSeconds == nullSeconds || loopEndSeconds == nullSeconds) &&
		"Did you pass a LoopType::Manual to the constructor? (BAD!)" );
	// if manual sample looping, sample inputs cannot be null
	assert( (loopType == LoopType::ManualSample) !=
		(loopStartSample == nullSample || loopEndSample == nullSample) &&
		"Did you pass a LoopType::Manual to the constructor? (BAD!)" );

	const auto IsFourCC = []( const BYTE* pData,const char* pFourcc )
	{
		assert( strlen( pFourcc ) == 4 );
		for( int i = 0; i < 4; i++ )
		{
			if( char( pData[i] ) != pFourcc[i] )
			{
				return false;
			}
		}
		return true;
	};

	unsigned int fileSize = 0;
	std::unique_ptr<BYTE[]> pFileIn;
	try
	{
		{
			std::ifstream file;
			file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
			file.open( fileName,std::ios::binary );

			{
				BYTE fourcc[4];
				file.read( reinterpret_cast<char*>(fourcc),4u );
				if( !IsFourCC( fourcc,"RIFF" ) )
				{
					throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"Bad fourcc code" );
				}
			}

			file.read( reinterpret_cast<char*>(&fileSize),sizeof( fileSize ) );
			fileSize += 8u; // entry doesn't include the fourcc or itself
			if( fileSize <= 44u )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"file too small" );
			}

			file.seekg( 0,std::ios::beg );
			pFileIn = std::make_unique<BYTE[]>( fileSize );
			file.read( reinterpret_cast<char*>(pFileIn.get()),fileSize );
		}

		if( !IsFourCC( &pFileIn[8],"WAVE" ) )
		{
			throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"format not WAVE" );
		}

		//look for 'fmt ' chunk id
		WAVEFORMATEX format;
		bool bFilledFormat = false;
		for( size_t i = 12u; i < fileSize; )
		{
			if( IsFourCC( &pFileIn[i],"fmt " ) )
			{
				memcpy( &format,&pFileIn[i + 8u],sizeof( format ) );
				bFilledFormat = true;
				break;
			}
			// chunk size + size entry size + chunk id entry size + word padding
			unsigned int chunkSize;
			memcpy( &chunkSize,&pFileIn[i + 4u],sizeof( chunkSize ) );
			i += (chunkSize + 9u) & 0xFFFFFFFEu;
		}
		if( !bFilledFormat )
		{
			throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"fmt chunk not found" );
		}

		// compare format with sound system format
		{
			const WAVEFORMATEX& sysFormat = SoundSystem::GetFormat();

			if( format.nChannels != sysFormat.nChannels )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad wave format (nChannels)" );
			}
			else if( format.wBitsPerSample != sysFormat.wBitsPerSample )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad wave format (wBitsPerSample)" );
			}
			else if( format.nSamplesPerSec != sysFormat.nSamplesPerSec )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad wave format (nSamplesPerSec)" );
			}
			else if( format.wFormatTag != sysFormat.wFormatTag )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad wave format (wFormatTag)" );
			}
			else if( format.nBlockAlign != sysFormat.nBlockAlign )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad wave format (nBlockAlign)" );
			}
			else if( format.nAvgBytesPerSec != sysFormat.nAvgBytesPerSec )
			{
				throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"bad wave format (nAvgBytesPerSec)" );
			}
		}

		//look for 'data' chunk id
		bool bFilledData = false;
		for( size_t i = 12u; i < fileSize; )
		{
			unsigned int chunkSize;
			memcpy( &chunkSize,&pFileIn[i + 4u],sizeof( chunkSize ) );
			if( IsFourCC( &pFileIn[i],"data" ) )
			{
				pData = std::make_unique<BYTE[]>( chunkSize );
				nBytes = chunkSize;
				memcpy( pData.get(),&pFileIn[i + 8u],nBytes );

				bFilledData = true;
				break;
			}
			// chunk size + size entry size + chunk id entry size + word padding
			i += (chunkSize + 9u) & 0xFFFFFFFEu;
		}
		if( !bFilledData )
		{
			throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"data chunk not found" );
		}

		switch( loopType )
		{
		case LoopType::AutoEmbeddedCuePoints:
			{
				looping = true;

				//look for 'cue' chunk id
				bool bFilledCue = false;
				for( size_t i = 12u; i < fileSize; )
				{
					unsigned int chunkSize;
					memcpy( &chunkSize,&pFileIn[i + 4u],sizeof( chunkSize ) );
					if( IsFourCC( &pFileIn[i],"cue " ) )
					{
						struct CuePoint
						{
							unsigned int cuePtId;
							unsigned int pop;
							unsigned int dataChunkId;
							unsigned int chunkStart;
							unsigned int blockStart;
							unsigned int frameOffset;
						};

						unsigned int nCuePts;
						memcpy( &nCuePts,&pFileIn[i + 8u],sizeof( nCuePts ) );
						if( nCuePts == 2u )
						{
							CuePoint cuePts[2];
							memcpy( cuePts,&pFileIn[i + 12u],sizeof( cuePts ) );
							loopStart = cuePts[0].frameOffset;
							loopEnd = cuePts[1].frameOffset;
							bFilledCue = true;
							break;
						}
					}
					// chunk size + size entry size + chunk id entry size + word padding
					i += (chunkSize + 9u) & 0xFFFFFFFEu;
				}
				if( !bFilledCue )
				{
					throw CHILI_SOUND_FILE_EXCEPTION( fileName,L"loop cue chunk not found" );
				}
			}
			break;
		case LoopType::ManualFloat:
			{
				looping = true;

				const WAVEFORMATEX& sysFormat = SoundSystem::GetFormat();
				const unsigned int nFrames = nBytes / sysFormat.nBlockAlign;

				const unsigned int nFramesPerSec = sysFormat.nAvgBytesPerSec / sysFormat.nBlockAlign;
				loopStart = unsigned int( loopStartSeconds * float( nFramesPerSec ) );
				assert( loopStart < nFrames );
				loopEnd = unsigned int( loopEndSeconds * float( nFramesPerSec ) );
				assert( loopEnd > loopStart && loopEnd < nFrames );

				// just in case ;)
				loopStart = std::min( loopStart,nFrames - 1u );
				loopEnd = std::min( loopEnd,nFrames - 1u );
			}
			break;
		case LoopType::ManualSample:
			{
				looping = true;

				const WAVEFORMATEX& sysFormat = SoundSystem::GetFormat();
				const unsigned int nFrames = nBytes / sysFormat.nBlockAlign;

				assert( loopStartSample < nFrames );
				loopStart = loopStartSample;
				assert( loopEndSample > loopStartSample && loopEndSample < nFrames );
				loopEnd = loopEndSample;

				// just in case ;)
				loopStart = std::min( loopStart,nFrames - 1u );
				loopEnd = std::min( loopEnd,nFrames - 1u );
			}
			break;
		case LoopType::AutoFullSound:
			{
				looping = true;

				const unsigned int nFrames = nBytes / SoundSystem::GetFormat().nBlockAlign;
				assert( nFrames != 0u && "Cannot auto full-loop on zero-length sound!" );
				loopStart = 0u;
				loopEnd = nFrames != 0u ? nFrames - 1u : 0u;
			}
			break;
		case LoopType::NotLooping:
			break;
		default:
			assert( "Bad LoopType encountered!" && false );
			break;
		}
	}
	catch( const SoundSystem::FileException& e )
	{
		nBytes = 0u;
		looping = false;
		pData.release();
		throw e;
	}
	catch( const std::exception& e )
	{
		nBytes = 0u;
		looping = false;
		pData.release();
		// needed for conversion to wide string
		const std::string what = e.what();
		throw CHILI_SOUND_FILE_EXCEPTION( fileName,std::wstring( what.begin(),what.end() ) );
	}
}

Sound::Sound( Sound&& donor )
{
	std::lock_guard<std::mutex> lock( donor.mutex );
	nBytes = donor.nBytes;
	donor.nBytes = 0u;
	looping = donor.looping;
	loopStart = donor.loopStart;
	loopEnd = donor.loopEnd;
	pData = std::move( donor.pData );
	activeChannelPtrs = std::move( donor.activeChannelPtrs );
	for( auto& pChan : activeChannelPtrs )
	{
		pChan->RetargetSound( &donor,this );
	}
	donor.cvDeath.notify_all();
}

Sound& Sound::operator=( Sound && donor )
{	
	// make sure nobody messes with our shit (also needed for cv.wait())
	std::unique_lock<std::mutex> lock( mutex );
	// check if there are even any active channels playing our jam
	if( activeChannelPtrs.size() != 0u )
	{
		// stop all channels currently playing our jam
		for( auto pChannel : activeChannelPtrs )
		{
			pChannel->Stop();
		}
		// wait for those channels to actually stop playing our jam
		cvDeath.wait( lock,[this] { return activeChannelPtrs.size() == 0u; } );
	}

	std::lock_guard<std::mutex> lock_donor( donor.mutex );
	nBytes = donor.nBytes;
	donor.nBytes = 0u;
	looping = donor.looping;
	loopStart = donor.loopStart;
	loopEnd = donor.loopEnd;
	pData = std::move( donor.pData );
	activeChannelPtrs = std::move( donor.activeChannelPtrs );	
	for( auto& pChan : activeChannelPtrs )
	{
		pChan->RetargetSound( &donor,this );
	}
	donor.cvDeath.notify_all();
	return *this;
}

void Sound::Play( float freqMod,float vol )
{
	SoundSystem::Get().PlaySoundBuffer( *this,freqMod,vol );
}

void Sound::StopOne()
{
	std::lock_guard<std::mutex> lock( mutex );
	if( activeChannelPtrs.size() > 0u )
	{
		activeChannelPtrs.front()->Stop();
	}
}

void Sound::StopAll()
{
	std::lock_guard<std::mutex> lock( mutex );
	for( auto pChannel : activeChannelPtrs )
	{
		pChannel->Stop();
	}
}

Sound::~Sound()
{
	// make sure nobody messes with our shit (also needed for cv.wait())
	std::unique_lock<std::mutex> lock( mutex );

	// check if there are even any active channels playing our jam
	if( activeChannelPtrs.size() == 0u )
	{
		return;
	}

	// stop all channels currently playing our jam
	for( auto pChannel : activeChannelPtrs )
	{
		pChannel->Stop();
	}

	// wait for those channels to actually stop playing our jam
	cvDeath.wait( lock,[this] { return activeChannelPtrs.size() == 0u; } );
}

SoundSystem::APIException::APIException( HRESULT hr,const wchar_t * file,unsigned int line,const std::wstring & note )
	:
	hr( hr ),
	ChiliException( file,line,note )
{}

std::wstring SoundSystem::APIException::GetFullMessage() const
{
	return L"Error Name: " + GetErrorName() + L"\n\n" +
		L"Error Description: " + GetErrorDescription() + L"\n\n" +
		L"Note: " + GetNote() + L"\n\n" +
		L"Location: " + GetLocation();
}

std::wstring SoundSystem::APIException::GetExceptionType() const
{
	return L"Sound System API Exception";
}

std::wstring SoundSystem::APIException::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring SoundSystem::APIException::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

SoundSystem::FileException::FileException( const wchar_t* file,unsigned int line,const std::wstring& note,const std::wstring& filename )
	:
	ChiliException( file,line,note ),
	filename( filename )
{}

std::wstring SoundSystem::FileException::GetFullMessage() const
{
	return L"Filename: " + filename + L"\n\n" +
		L"Note: " + GetNote() + L"\n\n" +
		L"Location: " + GetLocation();
}

std::wstring SoundSystem::FileException::GetExceptionType() const
{
	return L"Sound System File Exception";
}

SoundSystem::MFInitializer::MFInitializer()
{
	hr = MFStartup( MF_VERSION );
}

SoundSystem::MFInitializer::~MFInitializer()
{
	if( hr == S_OK )
	{
		MFShutdown();
	}
}
