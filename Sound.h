#include "SFML\Audio.hpp"
#include "Window.h"

class Sound
{
	sf::SoundBuffer dyng, basketball, volleyball;
	sf::Sound sound, soundDyng;

	Sound()
	{
		volleyball.loadFromFile("music/volleyball.wav");
		basketball.loadFromFile("music/basketball.wav");
		dyng.loadFromFile("music/dyng.wav");
	}

public:
	
	static Sound& getInstance()
	{
		static Sound s;
		return s;
	}

	Sound(const Sound& s) = delete;
	Sound& operator=(const Sound& s) = delete;

	void playBasketballMusic()
	{
		sound.setBuffer(basketball);
		sound.setLoop(true);
		sound.setVolume(50.f);
		sound.play();
	}

	void playVolleyballMusic()
	{
		sound.setBuffer(volleyball);
		sound.setLoop(true);
		sound.setVolume(50.f);
		sound.play();
	}

	void stopMusic()
	{
		sound.stop();
	}

	void playOneDyngSound()
	{
		soundDyng.setBuffer(dyng);
		soundDyng.setVolume(100.f);
		soundDyng.play();
	} 
};