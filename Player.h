#pragma once

#include "gameObject.h"
#include "Window.h"
#include "Container.h"
#include <stdlib.h>
#include <time.h>

class Player : public gameObject
{
	playerData dane;
	const bool isComputer;
	const std::string fileName = "volley/newblobbyFinal.png";
	sf::Texture texture;
public:

	// prawdziwy gracz
	Player(playerData& playerdata) : isComputer(false)
	{
		dane = playerdata;
		loadTexture();
	}

	// gracz_komputer
	Player() : isComputer(true)
	{
		srand(time(NULL));
		loadTexture();
	}

protected:
	void loadTexture()
	{
		texture.loadFromFile(fileName);
		texture.setSmooth(true);
		sprite.setTexture(texture);
		sprite.setPosition(sf::Vector2f(0.f, 0.f));
		sprite.setColor(dane.color);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	}

public:
	void drawObject(sf::RenderWindow& W)
	{
		sprite.setScale(Window::getMultiplier_Vector2f());
		W.draw(sprite);
		sprite.setScale(sf::Vector2f(1.f, 1.f));
	}

	std::string getName()
	{
		return (isComputer) ? "Computer" : dane.login;
	}

};
