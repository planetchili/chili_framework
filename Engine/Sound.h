/****************************************************************************************** 
 *	Chili DirectX Framework Sound Pack Version 16.11.11									  *	
 *	Sound.h																				  *
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
#pragma once
#include "ChiliWin.h"
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "ChiliException.h"
#include <wrl\client.h>
#include "COMInitializer.h"

// forward declare WAVEFORMATEX so we don't have to include bullshit headers
struct tWAVEFORMATEX;
typedef tWAVEFORMATEX WAVEFORMATEX;

class SoundSystem
{
public:
	class APIException : public ChiliException
	{
	public:
		APIException( HRESULT hr,const wchar_t * file,unsigned int line,const std::wstring& note );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
	class FileException : public ChiliException
	{
	public:
		FileException( const wchar_t* file,unsigned int line,const std::wstring& note,const std::wstring& filename );
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		std::wstring filename;
	};
private:
	class MFInitializer
	{
	public:
		MFInitializer();
		~MFInitializer();
	private:
		HRESULT hr;
	};
	class XAudioDll
	{
	private:
		enum class LoadType
		{
			Folder,
			Local,
			System,
			Invalid
		};
	public:
		XAudioDll();
		~XAudioDll();
		operator HMODULE() const;
	private:
		static const wchar_t* GetDllPath( LoadType type );
	private:
		HMODULE hModule = 0;
		static constexpr wchar_t* systemPath = L"XAudio2_7.dll";
#ifdef _M_X64
		static constexpr wchar_t* folderPath = L"XAudio\\XAudio2_7_64.dll";
		static constexpr wchar_t* localPath = L"XAudio2_7_64.dll";
#else
		static constexpr wchar_t* folderPath = L"XAudio\\XAudio2_7_32.dll";
		static constexpr wchar_t* localPath = L"XAudio2_7_32.dll";
#endif
	};
public:
	class Channel
	{
		friend class Sound;
	public:
		Channel( SoundSystem& sys );
		Channel( const Channel& ) = delete;
		~Channel();
		void PlaySoundBuffer( class Sound& s,float freqMod,float vol );
		void Stop();
	private:
		void RetargetSound( const Sound* pOld,Sound* pNew );
	private:
		std::unique_ptr<struct XAUDIO2_BUFFER> xaBuffer;
		struct IXAudio2SourceVoice* pSource = nullptr;
		class Sound* pSound = nullptr;
	};
public:
	SoundSystem( const SoundSystem& ) = delete;
	static SoundSystem& Get();
	static void SetMasterVolume( float vol = 1.0f );
	static const WAVEFORMATEX& GetFormat();
	void PlaySoundBuffer( class Sound& s,float freqMod,float vol );
private:
	SoundSystem();
	void DeactivateChannel( Channel& channel );
private:
	COMInitializer comInit;
	MFInitializer mfInit;
	XAudioDll xaudio_dll;
	Microsoft::WRL::ComPtr<struct IXAudio2> pEngine;
	struct IXAudio2MasteringVoice* pMaster = nullptr;
	std::unique_ptr<WAVEFORMATEX> format;
	std::mutex mutex;
	std::vector<std::unique_ptr<Channel>> idleChannelPtrs;
	std::vector<std::unique_ptr<Channel>> activeChannelPtrs;
private:
	// change these values to match the format of the wav files you are loading
	// all wav files must have the same format!! (no mixing and matching)
	static constexpr WORD nChannelsPerSound = 2u;
	static constexpr DWORD nSamplesPerSec = 44100u;
	static constexpr WORD nBitsPerSample = 16u;
	// change this value to increase/decrease the maximum polyphony	
	static constexpr size_t nChannels = 64u;
};

class Sound
{
	friend SoundSystem::Channel;
public:
	enum class LoopType
	{
		NotLooping,
		AutoEmbeddedCuePoints,
		AutoFullSound,
		ManualFloat,
		ManualSample,
		Invalid
	};
public:
	Sound() = default;
	// for backwards compatibility--2nd parameter false -> NotLooping (does not work with non-wav)
	Sound( const std::wstring& fileName,bool loopingWithAutoCueDetect );
	// do not pass this function Manual LoopTypes!
	Sound( const std::wstring& fileName,LoopType loopType = LoopType::NotLooping );
	Sound( const std::wstring& fileName,unsigned int loopStart,unsigned int loopEnd );
	Sound( const std::wstring& fileName,float loopStart,float loopEnd );
	Sound( Sound&& donor );
	Sound& operator=( Sound&& donor );
	void Play( float freqMod = 1.0f,float vol = 1.0f );
	void StopOne();
	void StopAll();
	~Sound();
private:
	static Sound LoadNonWav( const std::wstring& fileName,LoopType loopType,
							 unsigned int loopStartSample,unsigned int loopEndSample,
							 float loopStartSeconds,float loopEndSeconds );
	Sound( const std::wstring& fileName,LoopType loopType,
		unsigned int loopStartSample,unsigned int loopEndSample,
		float loopStartSeconds,float loopEndSeconds );
private:
	UINT32 nBytes = 0u;
	bool looping = false;
	unsigned int loopStart;
	unsigned int loopEnd;
	std::unique_ptr<BYTE[]> pData;
	std::mutex mutex;
	std::condition_variable cvDeath;
	std::vector<SoundSystem::Channel*> activeChannelPtrs;
	static constexpr unsigned int nullSample = 0xFFFFFFFFu;
	static constexpr float nullSeconds = -1.0f;
};