#include "Audio.h"
#include <iostream>
#include <filesystem>

using namespace std;
using namespace irrklang;
using namespace Engine;

AudioPlayerSingleton* AudioPlayerSingleton::m_instance;
string AudioPlayerSingleton::m_engineMediaPath;
string AudioPlayerSingleton::m_gameMediaPath;

AudioPlayerSingleton* AudioPlayerSingleton::getInstance()	
{
	if (m_instance == nullptr)
	{
		m_instance = new AudioPlayerSingleton();
		m_engineMediaPath = std::filesystem::current_path().parent_path().string() + "\\Engine\\src\\Engine\\Media\\";
		m_gameMediaPath = std::filesystem::current_path().parent_path().string() + "\\Game\\Assets\\";
	}

	return m_instance;
}

// Plays a sound, takes a path to the sound as a const char, a bool if the sound should loop,
// a bool if the sound should be initialized paused or play on initialization, and a bool for if
// sound effects should be used. Sound effects need to be added after using this function using
// the returned ISound*.  If the sound cannot be found, it returns a nullptr.
// If useFullPath is true, the full path to the sound file can be used. Otherwise, only the sound name
// needs to be put in with the file type. (E.g.:"GameEngine/Engine/src/Media/sound.wav" vs "sound.wav")
// More information on sound effects: https://www.ambiera.com/irrklang/docu/index.html#effects
// 
ISound* AudioPlayerSingleton::playSound(const char* soundNamePath,
	bool loop, bool startPaused, bool useSoundEffects, bool useFullPath)
{
	if (useFullPath)
	{
		ISound* theSound = engine->play2D(soundNamePath, loop, startPaused, true, ESM_AUTO_DETECT, useSoundEffects);
		if (theSound != 0)
		{
			return theSound;
		}
		return nullptr;
	}
	string filePath = m_engineMediaPath + soundNamePath;
	std::replace(filePath.begin(), filePath.end(), '\\', '/');
	const char* fullPath = filePath.c_str();
	ISound* theSound = engine->play2D(fullPath, loop,
		startPaused, true, ESM_AUTO_DETECT, useSoundEffects);
	if (theSound != 0)
	{
		return theSound;
	}
	string filePath2 = m_gameMediaPath;
	for (const auto& dEntry : std::filesystem::recursive_directory_iterator(filePath2))
	{
		if (dEntry.path().filename() == soundNamePath)
		{
			ISound* theSound2 = engine->play2D(dEntry.path().string().c_str(),
				loop, startPaused, true, ESM_AUTO_DETECT, useSoundEffects);
			if (theSound2 != 0)
			{
				return theSound2;
			}
			return nullptr;
		}
	}
	return nullptr;
}

// Plays a sound in 3D space, takes a const char* path to the sound, 
// an irrklang::vec3df position (irrklang::vec3df positionName(x,y,z)),
// a bool if the sound should loop, a bool if the sound should initialize paused,
// and a bool if the sound should use sound effects. Sound effects need to be added after
// using this funtion using the returned ISound*. If the sound cannot be found, it returns a nullptr.
// If useFullPath is true, the full path to the sound file can be used. Otherwise, only the sound name
// needs to be put in with the file type. (E.g.:"GameEngine/Engine/src/Media/sound.wav" vs "sound.wav")
// More information on sound effects: https://www.ambiera.com/irrklang/docu/index.html#effects
// 
ISound* AudioPlayerSingleton::play3DSound(const char* soundNamePath, vec3df sound3DPosition, bool loop, bool beginPaused, bool useSoundEffects, bool useFullPath)
{
	if (useFullPath)
	{
		ISound* theSound = engine->play3D(soundNamePath,
			sound3DPosition, loop, beginPaused, true, irrklang::ESM_AUTO_DETECT, useSoundEffects);
		if (theSound != 0)
		{
			return theSound;
		}
		return nullptr;
	}
	string filePath = m_engineMediaPath + soundNamePath;
	std::replace(filePath.begin(), filePath.end(), '\\', '/');
	const char* fullPath = filePath.c_str();
	ISound* theSound = engine->play3D(fullPath, sound3DPosition,
		loop, beginPaused, true, irrklang::ESM_AUTO_DETECT, useSoundEffects);
	if (theSound != 0)
	{
		return theSound;
	}
	string filePath2 = m_gameMediaPath;
	for (const auto& dEntry : std::filesystem::recursive_directory_iterator(filePath2))
	{
		if (dEntry.path().filename() == soundNamePath)
		{
			ISound* theSound2 = engine->play3D(dEntry.path().string().c_str(), sound3DPosition,
				loop, beginPaused, true, irrklang::ESM_AUTO_DETECT, useSoundEffects);
			if (theSound2 != 0)
			{
				return theSound2;
			}
			return nullptr;
		}
	}
	return nullptr;
}

// Use on an ISound* to clear memory space when the sound is no longer needed.
// Recommended but not necessary.
// 
void AudioPlayerSingleton::dropSound(ISound* sound)
{
	sound->drop();
	return;
}

// Call this function when finished.
//
void AudioPlayerSingleton::closeEngine()
{
	engine->drop();
	return;
}