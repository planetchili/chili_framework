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
#include "XAudio\XAudio2.h"
#include "DXErr.h"

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
	:
	Sound( fileName,loopType,nullSample,nullSample,nullSeconds,nullSeconds )
{
}

Sound::Sound( const std::wstring& fileName,unsigned int loopStart,unsigned int loopEnd )
	:
	Sound( fileName,LoopType::ManualSample,loopStart,loopEnd,nullSeconds,nullSeconds )
{
}

Sound::Sound( const std::wstring& fileName,float loopStart,float loopEnd )
	:
	Sound( fileName,LoopType::ManualFloat,nullSample,nullSample,loopStart,loopEnd )
{
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