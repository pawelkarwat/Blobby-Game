#include "Window.h"
#include "Pitch.h"

#define TIME_OF_BONUS_LIFE 1200

#define TIME_OF_RED_ITERATIONS 10

class Bonuses
{
	const std::string name;
	std::string content;
	int valueOfBonus;
	sf::Text text;
	sf::Font font;
	int iterations = 0;
	bool killTime = false;
	bool Player1_hit;
	bool endOfBonusLife = false;
	bool redTime = false;
	int iterationsOfRedTime = 0;

public:

	Bonuses(const std::string& type) : name(type)
	{
		setValue();
		loadTexture();
	}

protected:
	void loadTexture()
	{
		font.loadFromFile("fonts/OpenSans-Bold.ttf");
		text.setFont(font);
		text.setFillColor(sf::Color::Black);
		text.setPosition(sf::Vector2f((float)(200 + rand() % 2160) * Window::getMultiplier_X(), (float)(100 + rand() % 900) * Window::getMultiplier_Y()));
		text.setCharacterSize((unsigned)(40.f * Window::getMultiplier_Y()));
		content = name + " +" + std::to_string(valueOfBonus);
		text.setString(content);
	}

	void setValue()
	{
		valueOfBonus = 1 + rand() % 5;
	}

public:
	int getValue() const
	{
		return valueOfBonus;
	}

	const sf::FloatRect getGlobalBounds() const
	{
		return text.getGlobalBounds();
	}

	bool checkCollision(const sf::FloatRect& Bounds) const
	{
		return Bounds.intersects(text.getGlobalBounds());
	}

	std::string getName() const
	{
		return name;
	}

	void killingTimeStart(bool isPlayer1_hit)
	{
		this->Player1_hit = isPlayer1_hit;
		killTime = true;
		redTime = true;
		text.setFillColor(sf::Color::Red);
	}

	bool isPlayer1_hit() const
	{
		return Player1_hit;
	}

	bool iskillingTime() const
	{
		return killTime;
	}

	bool isEndOfBonusLife() const
	{
		return endOfBonusLife;
	}

	void drawObject(sf::RenderWindow& W)
	{
		if (!killTime || redTime)
		{
			W.draw(text);
			if (redTime)
			{
				iterationsOfRedTime++;
				if (iterationsOfRedTime == TIME_OF_RED_ITERATIONS)
					redTime = false;
			}
		}
		else
		{
			iterations++;
			if (iterations == TIME_OF_BONUS_LIFE)
				endOfBonusLife = true;
		}
	}
};