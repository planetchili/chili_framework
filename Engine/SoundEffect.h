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

class SoundEffect
{
public:
	SoundEffect( const std::initializer_list<std::wstring>& wavFiles,bool soft_fail = false,float freqStdDevFactor = 0.06f )
		:
		freqDist( 0.0f,freqStdDevFactor ),
		soundDist( 0,unsigned int( wavFiles.size() - 1 ) )
	{
		sounds.reserve( wavFiles.size() );
		for( auto& f : wavFiles )
		{
			try
			{
				sounds.emplace_back( f );
			}
			catch( const SoundSystem::FileException& e )
			{
				if( soft_fail )
				{
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
	void Play( T& rng,float vol = 1.0f )
	{
		sounds[soundDist( rng )].Play( exp2( freqDist( rng ) ),vol );
	}
private:
	std::uniform_int_distribution<unsigned int> soundDist;
	std::normal_distribution<float> freqDist;
	std::vector<Sound> sounds;
};