#pragma once
#include "Game.h"
#include "BasketballAI.h"

class Basketball : public Game
{
	bool ballisAboveBaskets;
public:
	Basketball(playerData data_1, playerData data_2, sf::RenderWindow& W) : Game(data_1, data_2, W, "basketball/tlo_3.png")
	{
		// ustawienie koszy
		setBakets();
		setSound();
	}
	Basketball(playerData data_1, sf::RenderWindow& W) : Game(data_1, W, "basketball/tlo_3.png")
	{
		// ustawienie koszy
		setBakets();
		setSound();
		Ai = new BasketballAI(pitch);
	}

	~Basketball()
	{
		sound.stopMusic();
		delete Ai;
	}

	void setBakets()
	{
		pitch.setWall(25.5f, 289.f, 25.f, 210.f);
		pitch.setWall(92.5f, 430.5f, 41.5f, 21.5f);
		pitch.setWall(433.f, 430.5f, 23.f, 21.5f);

		pitch.setWall(2560.f - 25.5f, 289.f, 25.f, 210.f);
		pitch.setWall(2560.f - 92.5f, 430.5f, 41.5f, 21.5f);
		pitch.setWall(2560.f - 433.f, 430.5, 23.f, 21.5f);
	}

	void setSound()
	{
		sound.playBasketballMusic();
	}



	// zwraca 0 brak punktów, 1 zapunktowal player1, 2 zapunktowal gracz 2
	int calculateResult()
	{
		int ret = 0;
		if (pitch.ball->pos.y < 480.f / pitch.getScale() && pitch.ball->pos.y > 370.f / pitch.getScale() && pitch.ball->pos.x > 134.f / pitch.getScale()
			&& pitch.ball->pos.x < 410.f / pitch.getScale() && ballisAboveBaskets)
		{
			++resultPlayer2;
			ret = 2;
		}
		if (pitch.ball->pos.y < 480.f / pitch.getScale() && pitch.ball->pos.y > 370.f / pitch.getScale() && pitch.ball->pos.x > 2151.f / pitch.getScale()
			&& pitch.ball->pos.y < 2422.f / pitch.getScale() && ballisAboveBaskets)
		{
			++resultPlayer1;
			ret = 1;
		}
		if (pitch.ball->pos.y < 370.f / pitch.getScale())
			ballisAboveBaskets = true;
		else
			ballisAboveBaskets = false;
		
		return ret;
	}

	void AiIteration()
	{
		Ai->MakeOneMove();
	}
};