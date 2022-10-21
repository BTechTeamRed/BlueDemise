#pragma once
#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "irrKlang.h"

namespace Engine 
{

	// A singleton for controlling sounds throughout the application.
	// Contacts: James
	//
	class AudioPlayerSingleton
	{
	private:
		static AudioPlayerSingleton* m_instance;

		AudioPlayerSingleton() = default;

	public:
		// Returns the instance of AudioPlayerSingleton
		static AudioPlayerSingleton* getInstance();

		irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice(irrklang::ESOD_DIRECT_SOUND_8, irrklang::ESEO_MULTI_THREADED);

		// Plays a simple sound in 2D, returning an irrklang::ISound to control the sound during play.
		irrklang::ISound* playSound(const char* soundName, bool loop = false, bool startPaused = false, bool useSoundEffects = false);
		// Plays a sound in 3D space, returning an irrklang::ISound to control the sound during play.
		irrklang::ISound* play3DSound(const char* soundName, irrklang::vec3df sound3DPosition, bool loop = false, bool beginPaused = false, bool useSoundEffects = false);
		// Clears memory space from a sound when finished using, taking a return of playSound() or play3DSound().
		void dropSound(irrklang::ISound* sound);
		// Use when finished with the sound engine (for the whole application).
		void closeEngine();

		AudioPlayerSingleton(const AudioPlayerSingleton&) = delete;
		AudioPlayerSingleton& operator=(const AudioPlayerSingleton&) = delete;

	};

}


#pragma region ExampleCode

// For further examples of how to use the ISoundEngine and
// ISound, view: https://www.ambiera.com/irrklang/docu/index.html

//#include "Audio.h"
//#include <iostream>
// 
//using namespace Engine;
//// use namespace irrklang to skip using "irrklang::"
//
//AudioPlayerSingleton* audioPlayer = AudioPlayerSingleton::getInstance();
//const char *soundPath = "/pathToSound";
////irrklang::ISound* theSound = audioPlayer->playSound(soundPath, true, false, false);
//irrklang::vec3df audioLocation(0, 0, 0); // if using a 3d sound
//irrklang::ISound* theSound = audioPlayer->play3DSound(soundPath, audioLocation, false, false);
//audioPlayer->engine->setListenerPosition(audioLocation, irrklang::vec3df(0, 1, 0)); // if using a 3d sound
//theSound->setMinDistance(5.0f);
//
//float vol = 0.5f;
//char i = 0;
//while (i != 'q')
//{
//    std::cout << "Press 'd' to decrease volume 10%, press 'u' to increase volume 10%,"
//              << "press 's' to drop the sound, press 'q' to quit.\n";
//    std::cin >> i; // wait for user to press some key
//    if (i == 'd')
//    {
//        vol -= 0.1f;
//        theSound->setVolume(vol);
//    }
//    if (i == 'u')
//    {
//        vol += 0.1f;
//        theSound->setVolume(vol);
//    }
//    if (i == 's')
//    {
//        audioPlayer->dropSound(theSound);
//    }
//}
//audioPlayer->closeEngine(); //Use before closing the application

#pragma endregion


#endif // !_AUDIO_H_