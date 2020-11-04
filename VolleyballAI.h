#pragma once

#include "AI.h"

class VolleyballAI : public AI
{
	sf::Vector2f ballPosition;
	sf::Vector2f player2_Position;
public:
	VolleyballAI(Pitch& pitch) : AI(pitch) {}

	void MakeOneMove()
	{
		ballPosition = sf::Vector2f(pitch.ball->pos.x * pitch.getScale(), pitch.ball->pos.y * pitch.getScale());
		player2_Position = sf::Vector2f(pitch.player_2->pos.x * pitch.getScale(), pitch.player_2->pos.y * pitch.getScale());
		if (ballPosition.y > 688.f && ballPosition.x < 1280.f)
			zone1();
		else if (isZone2(ballPosition))
			zone2_3();
		else if (isZone3(ballPosition))
			zone2_3();
		else
			zone4();
	}

private:
	void zone1()
	{
		if (player2_Position.x < 1770.f)
			pitch.setPlayer2_RIGHT();
		else if (player2_Position.x > 1990.f)
			pitch.setPlayer2_LEFT();
		else
			pitch.setPlayer2_NoMOVE();
	}

	void zone2_3()
	{
		float FuturePosition_x = calculateFuturePosition(200);
		if (FuturePosition_x < 1200.f)
			zone1();
		else
		{
			if (player2_Position.x > FuturePosition_x + 20.f)
				pitch.setPlayer2_LEFT();
			else if (player2_Position.x < FuturePosition_x - 20.f)
				pitch.setPlayer2_RIGHT();
			else
				pitch.setPlayer2_NoMOVE();
		}
	}

	void zone4()
	{
		float difference_x = player2_Position.x - ballPosition.x;
		if (player2_Position.x > 1850.f)
		{
			if (difference_x < 90.f)
				pitch.setPlayer2_RIGHT();
			else if (difference_x > 110.f)
				pitch.setPlayer2_LEFT();
			else
				pitch.setPlayer2_UP();
		}
		else
		{
			if (difference_x < 2.f)
				pitch.setPlayer2_RIGHT();
			else if (difference_x > 40.f)
				pitch.setPlayer2_LEFT();
			else
				pitch.setPlayer2_UP();
		}
	}

	float calculateFuturePosition(int numberOfIterations)
	{
		sf::Vector2f BallSpeed(pitch.ball->vel.x * pitch.getScale(), pitch.ball->vel.y * pitch.getScale());

		if (BallSpeed.x > 0.f || isZone3(ballPosition))
		{
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

				if (isZone2(predictPosition) || isZone4(predictPosition))
					break;
			}

			if (isZone4(predictPosition))
				return predictPosition.x;
		}
		return 0.f;
	}

	bool isZone1(const sf::Vector2f& wek2f) const
	{
		if (ballPosition.y > 688.f && ballPosition.x < 1280.f)
			return true;
		return false;
	}

	bool isZone4(const sf::Vector2f& wek2f) const
	{
		if (wek2f.x > 1280.f && wek2f.y > 688.f)
			return true;
		return false;
	}

	bool isZone2(const sf::Vector2f& wek2f)
	{
		if (wek2f.y < 688.f && wek2f.x < 1280.f)
			return true;
		return false;
	}

	bool isZone3(const sf::Vector2f& wek2f)
	{
		if (wek2f.y < 400.f && wek2f.x > 1280.f)
			return true;
		return false;
	}
};
