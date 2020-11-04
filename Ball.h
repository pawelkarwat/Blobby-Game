#pragma once

#include "gameObject.h"
#include "Window.h"

class Ball : public gameObject
{
	sf::Texture texture;
	const std::string fileName = "volley/newballFinal.png";
public:

	Ball()
	{
		loadTexture();
	}

	void loadTexture()
	{
		texture.loadFromFile(fileName);
		texture.setSmooth(true);
		sprite.setTexture(texture);
		sprite.setPosition(sf::Vector2f(0.f, 0.f));
		//sprite.setScale(Window::getMultiplier_Vector2f());
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	}

	void drawObject(sf::RenderWindow& W)
	{
		sprite.setScale(Window::getMultiplier_Vector2f());
		W.draw(sprite);
		sprite.setScale(sf::Vector2f(1.f, 1.f));
	}

	std::string getName()
	{
		return "ball";
	}

};

