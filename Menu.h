#pragma once

#include "Window.h"
#include "Container.h"
#include "Button.h"
#include "Volleyball.h"
#include "Basketball.h"
#include <string>
#include <sstream>
#include <vector>

#define VOLLEYBALL 0
#define BASKETBALL 1
#define SETTINGS 2
#define EXITBUTTON 3

class Menu
{
	const std::string fileName = "data.txt";
	playerData data_1, data_2;
	std::string excName, playerInput;
	sf::Text playerText;
	sf::Text text;
	sf::Font font;
	sf::Sprite bg;
	sf::Texture background;
	Button* Enterbutton, *Registrationbutton;
	Game* game;
	sf::RenderWindow& W;
	Sound& sound = Sound::getInstance();

	sf::Text text_statistics;

	int pointed = 0;
public:

	Menu(sf::RenderWindow& win) : W(win)
	{
		// loading data
		Container::load_data(fileName);
		font.loadFromFile("fonts/OpenSans-Bold.ttf");
		setFirstMenuDrawable();
	}

	~Menu()
	{
		Container::saveData(fileName);
		delete Enterbutton; delete Registrationbutton;
	}


	void drawMenu()
	{
		data_1 = TurnOnFirstMenu();
		std::string contentOfButtons[] = { "PLAY VOLLEYBALL", "PLAY BASKETBALL", "STATISTICS", "QUIT" };
		std::string contentOfButtons_2[] = { "SINGLE PLAYER", "MULTI PLAYER" };

		std::pair<int, int> result;
		bool loopContinue = true;
		while (loopContinue)
		{
			drawUniversalMenu(4, contentOfButtons);
			int firstChoice = pointed;
			int secondChoice;
			switch (firstChoice)
			{
			case VOLLEYBALL:
			case BASKETBALL:
				secondChoice = drawUniversalMenu(2, contentOfButtons_2);
				switch (pointed)
				{
				case 0:
					if (firstChoice == VOLLEYBALL)
					{
						game = new Volleyball(data_1, W);
						result = game->mainLoop();
						if (result != std::pair<int, int>(-1, -1))
							Container::updateStatistics(data_1.login, "Computer", "Volleyball", result.first, result.second, false);
					}
					else
					{
						game = new Basketball(data_1, W);
						result = game->mainLoop();
						if (result != std::pair<int, int>(-1, -1))
							Container::updateStatistics(data_1.login, "Computer", "Basketball", result.first, result.second, false);
					}
					delete game;
					break;
				case 1:
					data_2 = TurnOnFirstMenu();
					if (firstChoice == VOLLEYBALL)
					{
						game = new Volleyball(data_1, data_2, W);
						result = game->mainLoop();
						if (result != std::pair<int, int>(-1, -1))
							Container::updateStatistics(data_1.login, data_2.login, "Volleyball", result.first, result.second, true);
					}
					else
					{
						game = new Basketball(data_1, data_2, W);
						result = game->mainLoop();
						if (result != std::pair<int, int>(-1,-1))
							Container::updateStatistics(data_1.login, data_2.login, "Basketball", result.first, result.second, true);
					}

					delete game;
					break;
				}
				break;
			case SETTINGS:
				drawStatistics();
				break;
			case EXITBUTTON:
				W.close();
				loopContinue = false;
				break;
			}
		}
	}

private:
	void setFirstMenuDrawable()
	{
		background.loadFromFile("tlo_1/tlo1_2k.png");
		bg.setTexture(background);
		bg.setScale(Window::getMultiplier_Vector2f());
		bg.setPosition(sf::Vector2f(0.f, 0.f));

		Enterbutton = new Button("LOG IN", sf::Vector2f(1700.f * Window::getMultiplier_X(), 240.f * Window::getMultiplier_Y()), W);
		Registrationbutton = new Button("REGISTER", sf::Vector2f(1200.f *Window::getMultiplier_X(), 500.f * Window::getMultiplier_Y()), W);

		playerText.setFont(font);
		playerText.setFillColor(sf::Color::Black);
		playerText.setPosition(sf::Vector2f(950.f * Window::getMultiplier_X(), 250.f * Window::getMultiplier_Y()));
	}


	playerData TurnOnFirstMenu()
	{
		excName.clear(); playerInput.clear();
		playerData data;
		sf::Event e;
		bool error = false;

		while (W.isOpen())
		{
			while (W.pollEvent(e))
			{
				if (e.type == sf::Event::Closed)
				{
					W.close();
					exit(1);
				}
				if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::BackSpace)
				{
					sound.playOneDyngSound();
					if (playerInput.size() != 0)
						playerInput.pop_back();
				}
				else if (e.type == sf::Event::KeyPressed && playerInput.size() < 34)
				{
					sound.playOneDyngSound();
					char input = convert_to_Ascii(e.text.unicode);
					if (input != 0)
						playerInput += input;
				}
				// logowanie
				if ((e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) ||
					(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && Enterbutton->isMouseOnButton()))
				{
					error = false;
					try
					{
						data = Container::search(playerInput);
					}
					// not find the player
					catch (const char* exc)
					{
						error = true;
						playerInput.clear();
						excName = exc;
					}
					if (!error)
						return data;   // !!!!!
				}
				// rejestracja
				if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && Registrationbutton->isMouseOnButton())
				{
					error = false;
					try
					{
						data = Container::addNew(playerInput);
					}
					catch (const char* exc)
					{
						error = true;
						playerInput.clear();
						excName = exc;
					}
					if (!error)
						return data;
				}

			}

			Enterbutton->interactWithMouse();
			Registrationbutton->interactWithMouse();

			drawFirstMenu(error);
		}
		return data;
	}

	void drawFirstMenu(bool withError)
	{
		W.clear();
		W.draw(bg);
		drawStringEnter();
		playerText.setString(playerInput);
		W.draw(playerText);
		Enterbutton->draw(W);
		Registrationbutton->draw(W);
		if (withError) drawErrorText(excName);
		W.display();
	}

	char convert_to_Ascii(sf::Uint32 znak)
	{
		if (znak >= 0 && znak <= 25)
			znak += 65;
		else if (znak >= 26 && znak <= 35)
			znak += 22;
		else if (znak >= 75 && znak <= 84)
			znak -= 27;
		else
			znak = 0;
		return static_cast<char>(znak);
	}

	void drawStringEnter()
	{
		std::string s = "Enter your login (only letters and numbers):";
		sf::Font font;
		font.loadFromFile("fonts/OpenSans-Bold.ttf");

		text.setFont(font);
		text.setFillColor(sf::Color::Black);
		text.setPosition(sf::Vector2f(950.f * Window::getMultiplier_X(), 200.f * Window::getMultiplier_Y()));
		text.setString(s);

		W.draw(text);
	}

	void drawErrorText(const std::string& ExcText)
	{
		sf::Text errorText;
		errorText.setFont(font);
		errorText.setCharacterSize((unsigned)(40.f * Window::getMultiplier_Y()));
		errorText.setFillColor(sf::Color::Red);
		errorText.setPosition(710.f * Window::getMultiplier_X(), 800.f * Window::getMultiplier_Y());
		errorText.setString(ExcText);
		W.draw(errorText);
	}


	void initialsettingsMenu()
	{
		text_statistics.setFont(font);
		text_statistics.setFillColor(sf::Color::Black);
		text_statistics.setCharacterSize((unsigned)(40.f * Window::getMultiplier_Y()));
		text_statistics.setPosition(sf::Vector2f(500.f * Window::getMultiplier_X(), 300.f * Window::getMultiplier_Y()));
	}

	void drawStatistics()
	{
		sf::Event e;
		initialsettingsMenu();
		text_statistics.setString(getStatistics());
		
		while (W.isOpen())
		{
			while (W.pollEvent(e))
			{
				if (e.type == sf::Event::Closed)
				{
					W.close();
					exit(1);
				}
				if (e.type == sf::Event::KeyPressed)
					return;
			}
			W.clear();
			W.draw(bg);
			W.draw(text_statistics);
			W.display();
		}
	}

	std::string getStatistics()
	{
		std::stringstream ss;
		Container::update(data_1);
		ss << "Number of wins: " << data_1.numberOfWins << "\n";
		ss << "Number of loses: " << data_1.numberOfLoses << "\n";
		ss << "\nLast results:\n";
		for (auto& it : data_1.results)
		{
			ss << it.discipline << "  " << it.result_player << " : " << it.result_opponent << "  versus: " << it.opponent << "\n";
		}
		return ss.str();
	}

	int drawUniversalMenu(const int number_of_item, std::string* contentOfNumbers)
	{
		pointed = 0;
		sf::Event e;
		std::vector<Button*> buttons(number_of_item);
		createButtons(buttons, contentOfNumbers);

		while (W.isOpen())
		{
			while (W.pollEvent(e))
			{
				if (e.type == sf::Event::Closed)
				{
					W.close();
					exit(0);
				}
				if (e.type == sf::Event::KeyPressed && (e.key.code == sf::Keyboard::Down || e.key.code == sf::Keyboard::S))
				{
					sound.playOneDyngSound();
					if (pointed == number_of_item - 1)
						pointed = 0;
					else
						pointed++;
				}
				if (e.type == sf::Event::KeyPressed && (e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::W))
				{
					sound.playOneDyngSound();
					if (pointed == 0)
						pointed = number_of_item - 1;
					else
						pointed--;
				}
				if ((e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && buttons[pointed]->isMouseOnButton())
					|| (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter))
				{
					destructButtons(buttons);
					return pointed;
				}
			}
			for (int i = 0; i < number_of_item; i++)
			{
				if (buttons[i]->interactWithMouse())
					pointed = i;
			}
			adjustTexts(buttons);
			W.clear();
			W.draw(bg);
			for (int i = 0; i < number_of_item; i++)
				buttons[i]->draw(W);
			W.display();
		}
		destructButtons(buttons);
		return pointed;
	}

	void createButtons(std::vector<Button*>& buttons, std::string* contentOfButtons)
	{
		for (unsigned i = 0; i < buttons.size(); i++)
		{
			buttons[i] = new Button(contentOfButtons[i], sf::Vector2f(1150.f*Window::getMultiplier_X(), (200.f + (float)(200 * i))*Window::getMultiplier_Y()), W);
		}
	}

	void destructButtons(std::vector<Button*>& buttons)
	{
		for (int i = 0; i < buttons.size(); i++)
			delete buttons[i];
	}

	void adjustTexts(std::vector<Button*>& buttons)
	{
		for (unsigned i = 0; i < buttons.size(); i++)
		{
			buttons[i]->undoPointed();
		}
		buttons[pointed]->setPointed();
	}
};