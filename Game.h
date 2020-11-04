#pragma once
#include <Box2D\Box2D.h>
#include "Window.h"
#include "Container.h"
#include "Pitch.h"
#include "AI.h"

class Game
{
protected:
	const bool singlePlayer;
	Pitch pitch;
	Sound& sound = Sound::getInstance();
	sf::RenderWindow& W;
	int resultPlayer1 = 0, resultPlayer2 = 0;
	AI* Ai;
public:
	Game(playerData data_1, playerData data_2, sf::RenderWindow& W, const std::string& fileBoardName) : pitch(Pitch(data_1, data_2, fileBoardName)), W(W), singlePlayer(false) {}

	Game(playerData data_1, sf::RenderWindow& W, const std::string& fileBoardName) :pitch(Pitch(data_1, fileBoardName)), W(W), singlePlayer(true) {}



	std::pair<int, int> mainLoop()
	{
		while (W.isOpen())
		{
			sf::Event e;
			while (W.pollEvent(e))
			{
				if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					return std::pair<int, int>(-1, -1);
			}
			if (singlePlayer)
				AiIteration();

			pitch.makeIteration();

			W.clear();

			pitch.drawUpdatePositions(W);

			int which_scores = calculateResult();
			switch (which_scores)
			{
			case 1:
				pitch.resetPosition(false);
				break;
			case 2:
				pitch.resetPosition(true);
			}

			pitch.drawResult(resultPlayer1, resultPlayer2, W);

			if (checkIfEnd())
			{
				if (resultPlayer1 > resultPlayer2)
					pitch.drawEndMessage(true, W);
				else
					pitch.drawEndMessage(false, W);
				return std::pair<int, int>(resultPlayer1, resultPlayer2);
			}

			W.display();
		}
	}

	bool checkIfEnd()
	{
		if (resultPlayer1 == 15)
		{
			pitch.drawEndMessage(true, W);
			return true;
		}
		if (resultPlayer2 == 15)
		{
			pitch.drawEndMessage(false, W);
			return true;
		}
		return false;
	}

	virtual void setSound() = 0;

	virtual int calculateResult() = 0;

	virtual void AiIteration() = 0;
};
