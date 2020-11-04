#pragma once

#include "Game.h"
#include "VolleyballAI.h"


class Volleyball : public Game
{
public:
	Volleyball(playerData data_1, playerData data_2, sf::RenderWindow& W) : Game(data_1, data_2, W, "volley/bg_volleyball.png")
	{
		// ustawienie siatki
		pitch.setWall(1280.f, 1080.f, 32.f, 408.f);
		setSound();
	}
	Volleyball(playerData data_1, sf::RenderWindow& W) : Game(data_1, W, "volley/bg_volleyball.png")
	{
		// ustawienie siatki
		pitch.setWall(1280.f, 1080.f, 32.f, 408.f);
		setSound();
		Ai = new VolleyballAI(pitch);
	}

	~Volleyball()
	{
		sound.stopMusic();
		delete Ai;
	}

	void setSound()
	{
		sound.playVolleyballMusic();
	}

	// zwraca 0 brak punktów, 1 zapunktowal player1, 2 zapunktowal gracz 2
	int calculateResult()
	{
		if (pitch.ball->pos.y > 37.8f)
		{
			if (pitch.ball->pos.x > 41.f)
			{
				resultPlayer1++;
				return 1;
			}
			resultPlayer2++;
			return 2;
		}
		return false;
	}

	void AiIteration()
	{
		Ai->MakeOneMove();
	}

};
