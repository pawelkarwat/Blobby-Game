#pragma once

#include "SFML/Graphics.hpp"

class Window
{
public:
	static const unsigned X;
	static const unsigned Y;

	static float getMultiplier_X()
	{
		return ((float) X) / 2560.f;
	}
	static float getMultiplier_Y()
	{
		return ((float) Y) / 1440.f;
	}
	static sf::Vector2f getMultiplier_Vector2f()
	{
		return sf::Vector2f(getMultiplier_X(), getMultiplier_Y());
	}

	static void configureWindow(sf::RenderWindow& W)
	{
		W.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Volley-basket-kuszka", sf::Style::Fullscreen);
		W.setActive(true);
		W.setVerticalSyncEnabled(true);
		W.setKeyRepeatEnabled(true);
		W.setPosition(sf::Vector2i(0, 0));
	}
};

const unsigned Window::X = sf::VideoMode::getDesktopMode().width;
const unsigned Window::Y = sf::VideoMode::getDesktopMode().height;
