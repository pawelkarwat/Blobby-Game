#pragma once

#include "AI.h"

class BasketballAI : public AI
{
	sf::Vector2f ballPosition;
	sf::Vector2f player2_Position;

public:
	BasketballAI(Pitch& pitch) : AI(pitch) {}

	void MakeOneMove()
	{
		ballPosition = sf::Vector2f(pitch.ball->pos.x * pitch.getScale(), pitch.ball->pos.y * pitch.getScale());
		player2_Position = sf::Vector2f(pitch.player_2->pos.x * pitch.getScale(), pitch.player_2->pos.y * pitch.getScale());
		if (ballPosition.y < 400.f)
			highZone();
		else
			lowZone();
	}

private:
	void highZone()
	{
		float FuturePosition_x = calculateFuturePosition(200);
		
		if (player2_Position.x > FuturePosition_x + 20.f)
			pitch.setPlayer2_LEFT();
		else if (player2_Position.x < FuturePosition_x - 20.f)
			pitch.setPlayer2_RIGHT();
		else
			pitch.setPlayer2_NoMOVE();
	}

	void lowZone()
	{
		float difference_x = player2_Position.x - ballPosition.x;
		if (ballPosition.x < 450.f)
		{
			if (difference_x < -5.f)
				pitch.setPlayer2_RIGHT();
			else if (difference_x > 5.f)
				pitch.setPlayer2_LEFT();
			else
				pitch.setPlayer2_UP();
		}
		else if (ballPosition.x < 657.f)
		{
			if (difference_x < 3.f)
				pitch.setPlayer2_RIGHT();
			else if (difference_x > 20.f)
				pitch.setPlayer2_LEFT();
			else
				pitch.setPlayer2_UP();
		}
		else if (ballPosition.x < 1280.f)
		{
			if (difference_x < 20.f)
				pitch.setPlayer2_RIGHT();
			else if (difference_x > 40.f)
				pitch.setPlayer2_LEFT();
			else
				pitch.setPlayer2_UP();
		}
		else if (ballPosition.x < 1904.f)
		{
			if (difference_x < 30.f)
				pitch.setPlayer2_RIGHT();
			else if (difference_x > 50.f)
				pitch.setPlayer2_LEFT();
			else
				pitch.setPlayer2_UP();
		}
		else
		{
			if (difference_x < 50.f)
				pitch.setPlayer2_RIGHT();
			else if (difference_x > 75.f)
				pitch.setPlayer2_LEFT();
			else
				pitch.setPlayer2_UP();
		}
	}

	float calculateFuturePosition(int numberOfIterations)
	{
		sf::Vector2f BallSpeed(pitch.ball->vel.x * pitch.getScale(), pitch.ball->vel.y * pitch.getScale());

		const float timeUnit = 0.16666666666667f;
		sf::Vector2f predictPosition = ballPosition;
		sf::Vector2f predictSpeed = BallSpeed;
		sf::Vector2f gravity = pitch.getGravity();
		bool changed = false;

		for (int i = 0; i < numberOfIterations; i++)
		{
			predictPosition = predictPosition + sf::Vector2f(predictSpeed.x * timeUnit, predictSpeed.y * timeUnit + gravity.y * timeUnit * timeUnit * 0.5f);
			predictSpeed = predictSpeed + sf::Vector2f(0.f, gravity.y * timeUnit);
			if ((predictPosition.x > 2420.f || predictPosition.x < 38.f) && !changed)
			{
				changed = true;
				predictSpeed = sf::Vector2f(-predictSpeed.x, -predictSpeed.y);
			}
			if (ballPosition.y < 400.f)
				break;
		}

		return predictPosition.x;
	}
};