#pragma once

#include "Menu.h"
#include <thread>


class Program
{
	sf::RenderWindow W;
	Menu* menu;
public:
	Program()
	{
		Window::configureWindow(W);
		start();
	}

	void start()
	{
		std::thread t(&Program::allocate, this);
		intro();
		t.join();
		menu->drawMenu();
		delete menu;
	}

	void allocate()
	{
		menu = new Menu(W);
	}

	void intro()
	{
		sf::Event e;
		sf::Texture texture;
		texture.loadFromFile("tlo_1/tlo0_2k.png");
		sf::Sprite spr;
		spr.setTexture(texture);
		spr.setScale(Window::getMultiplier_Vector2f());
		spr.setPosition(sf::Vector2f(0.f, 0.f));
		for (int i = 0; i < 100 && W.isOpen(); i++)
		{
			while (W.pollEvent(e))
			{
				if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					W.close();
					exit(0);
				}
			}
			spr.setScale(sf::Vector2f(1.f / 2000.f + spr.getScale().x, 1.f / 2000.f + spr.getScale().y));
			//spr.setPosition(spr.getPosition() + sf::Vector2f( Window::getMultiplier_X() / -500.f, Window::getMultiplier_Y() / -500.f));
			spr.move(sf::Vector2f(Window::X / -3000.f, Window::Y / -2000.f));

			//spr.setTextureRect(sf::IntRect(Window::X))
			
			W.clear(sf::Color::Black);
			W.draw(spr);
			W.display();
		}

	}
};
