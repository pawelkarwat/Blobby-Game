#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "Sound.h"

class Button : public sf::Text
{
	sf::Font font;
	Sound& sound = Sound::getInstance();
	sf::RenderWindow& Window;
	sf::Texture pointerTexture;
	sf::Sprite pointer;
	bool PointedFlag;

	void configure()
	{
		font.loadFromFile("fonts/OpenSans-Bold.ttf");
		this->setFont(font);
		this->setFillColor(sf::Color::Black);
		if (isMouseOnButton())
			PointedFlag = true;
		else
			PointedFlag = false;
		
		pointerTexture.loadFromFile("tlo_1/Pointer.png");
	}

public:
	Button(const std::string& content, const sf::Vector2f& Position, sf::RenderWindow& W) : Window(W)
	{
		configure();
		create(content, Position);
	}

	Button(sf::RenderWindow& W) : Window(W)
	{
		configure();
	}

	void create(const std::string& content, const sf::Vector2f& Position)
	{
		
		this->setCharacterSize((unsigned)(40.f*Window::getMultiplier_Y()));
		this->setPosition(Position);
		this->setString(content);
		
		pointer.setTexture(pointerTexture);
		pointer.setPosition(Position - sf::Vector2f(50.f, -5.f));
		pointer.setScale(Window::getMultiplier_Vector2f());
	}

	void setPointed()
	{
		this->setFillColor(sf::Color::Red);
		PointedFlag = true;
	}

	void undoPointed()
	{
		this->setFillColor(sf::Color::Black);
		PointedFlag = false;
	}

	// 1 - if it interact
	bool interactWithMouse()
	{
		if (isMouseOnButton())
		{
			this->setFillColor(sf::Color::Red);
			if (!PointedFlag)
			{
				PointedFlag = true;
				Sound& sound = Sound::getInstance();
				sound.playOneDyngSound();
			}
			return true;
		}
		this->setFillColor(sf::Color::Black);
		PointedFlag = false;
		return false;
	}

	bool isMouseOnButton() const
	{
		return this->getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(Window));
	}

	void draw(sf::RenderWindow& W)
	{
		if (PointedFlag == true)
			W.draw(pointer);
		W.draw(*this);
	}
};