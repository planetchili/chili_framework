/******************************************************************************************
 *	Chili DirectX Framework Sound Pack Version 16.11.11									  *
 *	SoundEffect.h																		  *
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
#include "Sound.h"
#include <random>
#include <initializer_list>
#include <memory>
#include <fstream>
#include <cassert>

class SoundEffect
{
public:
	// this ctor reads from a .sfx file to configure/load a sound effect
	SoundEffect(const std::wstring& filename)
	{
		std::wifstream sfxFile(filename);
		// first line is the freq stddev
		float freqStdDevFactor;
		sfxFile >> freqStdDevFactor;
		sfxFile.ignore();
		// remaining lines are the sound files
		std::vector<std::wstring> soundFileNames;
		for (std::wstring s; std::getline(sfxFile, s); )
		{
			soundFileNames.push_back(std::move(s));
		}
		// now load the dumb sound effect matrix
		*this = SoundEffect(std::move(soundFileNames), true, freqStdDevFactor);
	}
	SoundEffect(std::vector<std::wstring> wavFiles, bool soft_fail = false, float freqStdDevFactor = 0.06f)
		:
		freqDist(0.0f, freqStdDevFactor),
		soundDist(0, unsigned int(wavFiles.size() - 1))
	{
		sounds.reserve(wavFiles.size());
		for (auto& f : wavFiles)
		{
			try
			{
				sounds.emplace_back(f);
			}
			catch (const SoundSystem::FileException& e)
			{
				if (soft_fail)
				{
#ifndef NDEBUG
					// throw anyways if in debug (we devs wanna know!)
					throw e;
#endif
					sounds.emplace_back();
				}
				else
				{
					throw e;
				}
			}
		}
	}
	template<class T>
	void Play(T& rng, float vol = 1.0f) const
	{
		sounds[soundDist(rng)].Play(exp2(freqDist(rng)), vol);
	}
	// NOT THREAD SAFE!
	// calls main play function with default rng
	void Play(float vol = 1.0f) const
	{
		Play(defaultRng, vol);
	}
private:
	// make distribs mutable so that Play can be const
	// (dist call not really a substantial mutation anyways...)
	mutable std::uniform_int_distribution<unsigned int> soundDist;
	mutable std::normal_distribution<float> freqDist;
	std::vector<Sound> sounds;
	// global default rng for sound effects
	// not thread safe!
	static std::mt19937 defaultRng;
};