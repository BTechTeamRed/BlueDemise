#include "Audio.h"

using namespace std;
using namespace irrklang;
using namespace Engine;

AudioPlayerSingleton* AudioPlayerSingleton::m_instance;

AudioPlayerSingleton* AudioPlayerSingleton::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new AudioPlayerSingleton();
	}

	return m_instance;
}

// Plays a sound, takes a path to the sound as a const char, a bool if the sound should loop,
// a bool if the sound should be initialized paused or play on initialization, and a bool for if
// sound effects should be used. Sound effects need to be added after using this function using
// the returned ISound*.
// More information on sound effects: https://www.ambiera.com/irrklang/docu/index.html#effects
// 
ISound* AudioPlayerSingleton::playSound(const char* soundPath, bool loop, bool startPaused, bool useSoundEffects)
{
	ISound* theSound = engine->play2D(soundPath, loop, startPaused, true, ESM_AUTO_DETECT, useSoundEffects);
	if (theSound != 0)
	{
		return theSound;
	}
	return nullptr;
	//TODO: implement alternate media folder search if multiple media folders
	// (for game and engine for example) are implemented
}

// Plays a sound in 3D space, takes a const char* path to the sound, 
// an irrklang::vec3df position (irrklang::vec3df positionName(x,y,z)),
// a bool if the sound should loop, a bool if the sound should initialize paused,
// and a bool if the sound should use sound effects. Sound effects need to be added after
// using this funtion using the returned ISound*.
// More information on sound effects: https://www.ambiera.com/irrklang/docu/index.html#effects
// 
ISound* AudioPlayerSingleton::play3DSound(const char* soundPath, vec3df sound3DPosition, bool loop, bool beginPaused, bool useSoundEffects)
{
	ISound* theSound = engine->play3D(soundPath, sound3DPosition, loop, beginPaused, true, irrklang::ESM_AUTO_DETECT, useSoundEffects);
	if (theSound != 0)
	{
		return theSound;
	}
	return nullptr;
	//TODO: implement alternate media folder search if multiple media folders
	// (for game and engine for example) are implemented
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