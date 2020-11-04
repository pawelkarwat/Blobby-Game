#pragma once

#include "Game.h"
#include "Ball.h"
#include "Player.h"
#include "Bonuses.h"
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>

// prawdopodobieñstwo w skali 0 - 1000
#define PROPABILITY_OF_BONUS 5

#define ITERATIONS_BONUS_IS_AVAILABLE 60 * 10

#define TIME_OF_BONUS

enum Direction{nothing, left, right, up};

class Pitch
{
	sf::Font font;
	sf::Text text, text_end;
	bool single_player;
	sf::Texture board_texture;
	const std::string fileName;
	const b2Vec2 Gravity;
	b2PolygonShape shape;
	b2BodyDef bdef;
	b2ContactListener listener;
	char* names[3];
	sf::Sprite board;
	sf::Texture texture;
	const float SCALE = 30.f;
	const float DEG = 57.29577f;
	b2Body* b;
	b2Body * playersBody[2];
	b2World World;
	Bonuses* bonus = nullptr;
	int iterationsOfBonus = 0;
	bool player_1_lastPickUp = true;
	int number_of_pickUp = 0;
	Sound& sound = Sound::getInstance();
	Direction direction_player2 = nothing;

	const std::string speedBonusName = "SPEED";
	const std::string massBonusName = "MASS";
	const std::string jumpBonusName = "JUMP";

	float horizontalSpeed_player1 = 12.f;
	float horizontalSpeed_player2 = 12.f;
	float jumpSpeed_player1 = 16.f;
	float jumpSpeed_player2 = 16.f;

	b2MassData b2MassData_player1, b2MassData_player2;
public:
	gameObject* player_1, *player_2, *ball;



private:
	void initialize_players()
	{
		for (int i = 0; i < 2; i++)
		{
			bdef.position.Set((i == 0) ? 19.7917f : 64.4475f, 37.0617f);
			b2CircleShape circle;
			circle.m_radius = 102.4f / SCALE;
			circle.m_p.Set(0.f, 41.6f / SCALE);
			playersBody[i] = World.CreateBody(&bdef);
			playersBody[i]->CreateFixture(&circle, 5.f);
			circle.m_radius = 80.f / SCALE;
			circle.m_p.Set(0.f, -64.f / SCALE);
			playersBody[i]->CreateFixture(&circle, 5.f);
			playersBody[i]->SetFixedRotation(true);
		}
		playersBody[0]->SetUserData((void*)(names[0] = const_cast<char*>(player_1->getName().c_str())));
		playersBody[1]->SetUserData((void*)(names[1] = const_cast<char*>(player_2->getName().c_str())));
	}

	void initialize_ball()
	{
		b2CircleShape circle;
		bdef.position.Set(19.7917f, 10.f);
		circle.m_radius = 102.4f / SCALE;
		b = World.CreateBody(&bdef);
		b2FixtureDef fdef;
		fdef.shape = &circle;
		fdef.restitution = 0.95f;
		fdef.density = 0.2f;
		b->CreateFixture(&fdef);
		b->SetUserData((void*)(names[2] = const_cast<char*>(ball->getName().c_str())));
	}

public:

	Pitch(playerData& playerdata_1, playerData& playerdata_2, const std::string& file) : Gravity(0.f, 9.8f), World(Gravity), fileName(file)
	{
		srand(time(NULL));
		single_player = false;
		player_1 = new Player(playerdata_1);
		player_2 = new Player(playerdata_2);
		ball = new Ball();
		//for (int i = 0; i < 3; i++)
		//	names[i] = new char[200];
		initialize_pitch();
	}
	Pitch(playerData& playerdata_1, const std::string& file) : Gravity(0.f, 9.8f), World(Gravity), fileName(file)
	{
		srand(time(NULL));
		single_player = true;
		player_1 = new Player(playerdata_1);
		player_2 = new Player();
		ball = new Ball();
		//for (int i = 0; i < 3; i++)
		//	names[i] = new char[200];
		initialize_pitch();
	}

	~Pitch()
	{
		sound.stopMusic();
		delete player_1;
		delete player_2;
		delete ball;
		if (bonus != nullptr)
			delete bonus;
	}

private:

	void initialize_pitch()
	{
		World.SetContactListener(&listener);
		initialize_texts();
		load_sprites();
		setWall(1280.f, 1280.f, 6400.f, 24.f);
		setWall(0.f, 0.f, 32.f, 4800.f);
		setWall(2560.f, 0.f, 32.f, 4800.f);

		// !!!!!!!!!!!!!!!!!!!!!!!!!
		shape.SetAsBox(96 / SCALE, 72 / SCALE);
		bdef.type = b2_dynamicBody;

		initialize_players();
		initialize_ball();
	}

	void initialize_texts()
	{
		font.loadFromFile("fonts/OpenSans-Bold.ttf");

		// koñcowy napis
		text_end.setFont(font);
		text_end.setFillColor(sf::Color::Red);
		text_end.setOrigin(sf::Vector2f(0.f, 0.f));
		text_end.setPosition(200.f * Window::getMultiplier_X(), 50.f * Window::getMultiplier_Y());
		text_end.setCharacterSize((unsigned)(60.f*Window::getMultiplier_Y()));

		// napis wyœwietlaj¹cy rezultat
		text.setFont(font);
		text.setFillColor(sf::Color::Red);
		text.setOrigin(sf::Vector2f(0.f, 0.f));
		text.setPosition(200.f * Window::getMultiplier_X(), 50.f * Window::getMultiplier_Y());
		text.setCharacterSize((unsigned)(40.f*Window::getMultiplier_Y()));
	}


	void load_sprites()
	{
		// sf::Texture texture;
		texture.loadFromFile(fileName);
		texture.setSmooth(true);
		board.setTexture(texture);
		board.setPosition(sf::Vector2f(0.f, 0.f));
		board.setScale(Window::getMultiplier_Vector2f());
	}

public:
	float getScale() const
	{
		return SCALE;
	}

	int getNumberOfPickUps_player1() const
	{
		return (player_1_lastPickUp) ? number_of_pickUp : 0;
	}

	int getNumberOfPickUps_player2() const
	{
		return (player_1_lastPickUp) ? 0 : number_of_pickUp;
	}

	sf::Vector2f getGravity() const
	{
		return sf::Vector2f(Gravity.x * SCALE, Gravity.y * SCALE);
	}

	void setWall(float x, float y, float width, float height)
	{
		b2PolygonShape gr;
		gr.SetAsBox(width / SCALE, height / SCALE);

		b2BodyDef bdef;
		bdef.position.Set(x / SCALE, y / SCALE);

		b2Body* b_ground = World.CreateBody(&bdef);
		b_ground->CreateFixture(&gr, 1.f);
	}

	void setPlayer2_UP()
	{
		direction_player2 = up;
	}

	void setPlayer2_LEFT()
	{
		direction_player2 = left;
	}

	void setPlayer2_RIGHT()
	{
		direction_player2 = right;
	}

	void setPlayer2_NoMOVE()
	{
		direction_player2 = nothing;
	}

	void setPlayer1Velocity()
	{
		player_1->pos = playersBody[0]->GetPosition();
		player_1->vel = playersBody[0]->GetLinearVelocity();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player_1->pos.x < 80.8f)
			player_1->vel.x = horizontalSpeed_player1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player_1->pos.x > 4.5f)
			player_1->vel.x = (-1.f) * horizontalSpeed_player1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			if (player_1->pos.y >= 35.9)
				player_1->vel.y = (-1.f) * jumpSpeed_player1;
		if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
			player_1->vel.x = 0;

		playersBody[0]->SetLinearVelocity(player_1->vel);
	}

	void setPlayer2Velocity()
	{
		player_2->pos = playersBody[1]->GetPosition();
		player_2->vel = playersBody[1]->GetLinearVelocity();

		if (!single_player)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player_2->pos.x < 80.8f)
				player_2->vel.x = horizontalSpeed_player2;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player_2->pos.x > 4.5f)
				player_2->vel.x = (-1.f) * horizontalSpeed_player2;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				if (player_2->pos.y >= 35.9)
					player_2->vel.y = (-1.f) * jumpSpeed_player2;
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
				player_2->vel.x = 0;
		}
		else
		{
			if (direction_player2 == right && player_2->pos.x < 80.8f)
				player_2->vel.x = horizontalSpeed_player2;
			if (direction_player2 == left && player_2->pos.x > 4.5f)
				player_2->vel.x = (-1.f) * horizontalSpeed_player2;
			if (direction_player2 == up)
				if (player_2->pos.y >= 35.9)
					player_2->vel.y = (-1.f) * jumpSpeed_player2;
			if (!(direction_player2 == right || direction_player2 == left))
				player_2->vel.x = 0;
		}
		playersBody[1]->SetLinearVelocity(player_2->vel);
	}

	void makeIteration()
	{
		// predkosc 2
		for (int n = 0; n < 2; n++)
		{
			checkPickUpAndCollision();
			World.Step(1 / 60.f, 8, 3);
		}
		iterationsOfBonus++;
		if (bonus != nullptr && iterationsOfBonus == ITERATIONS_BONUS_IS_AVAILABLE && !bonus->iskillingTime())
		{
			delete bonus;
			bonus = nullptr;
		}
		setBonus();

		setPlayer1Velocity();
		setPlayer2Velocity();

		// ball max speed
		
		ball->vel = b->GetLinearVelocity();
		if (ball->vel.Length() > 36)
			b->SetLinearVelocity(36 / ball->vel.Length() * ball->vel);
	}

private:
	
	void checkPickUpAndCollision()
	{
		bool helpFlag = false;
		static const int SIZE = 30;
		static bool help[SIZE];

		for (b2Contact* it = World.GetContactList(); it != 0; it = it->GetNext())
		{
			b2Body* body_A = it->GetFixtureA()->GetBody();
			b2Body* body_B = it->GetFixtureB()->GetBody();

			if (body_B->GetUserData() == (void*)names[2])
			{
				if (body_A->GetUserData() == (void*)names[0] && it->IsTouching())
				{

					sound.playOneDyngSound();
					if (player_1_lastPickUp)
						number_of_pickUp++;
					else
					{
						player_1_lastPickUp = true;
						number_of_pickUp = 1;
					}
				}
				else if (body_A->GetUserData() == (void*)names[1] && it->IsTouching())
				{
					sound.playOneDyngSound();
					if (player_1_lastPickUp)
					{
						player_1_lastPickUp = false;
						number_of_pickUp = 1;
					}
					else
						number_of_pickUp++;
				}
			}
		}
	}


	void setBonus()
	{
		if (bonus == nullptr)
		{
			//prawdopodobieñstwo pojawienia siê bonusu
			if (rand() % 1000 < PROPABILITY_OF_BONUS)
			{
				int draw = rand() % 3;
				switch (draw)
				{
				case 0:
					bonus = new Bonuses(speedBonusName);
					break;
				case 1:
					bonus = new Bonuses(jumpBonusName);
					break;
				case 2:
					bonus = new Bonuses(massBonusName);
					break;
				}
				iterationsOfBonus = 0;
			}
		}
		else
		{
			if (bonus->isEndOfBonusLife())
			{
				takeBonus();
				delete bonus;
				bonus = nullptr;
			}
			else if (!bonus->iskillingTime())
			{
				int x = checkHitOfBonus();
				switch (x)
				{
				case 1:
					giveBonus(true);
					bonus->killingTimeStart(true);
					break;
				case 2:
					giveBonus(false);
					bonus->killingTimeStart(false);
					break;
				}
			}
		}
	}

	// zwraca 0 gdy nikt nie zbil, 1 gdy player 1 zbil, 2 gdy player 2 zbil, oraz ustawia odpowiednie wartoœci
	int checkHitOfBonus()
	{
		if (bonus->checkCollision(ball->sprite.getGlobalBounds()))
		{
			if (player_1_lastPickUp)
				return 1;
			return 2;
		}
		return 0;
	}


	void giveBonus(bool to_player_1)
	{
		if (bonus == nullptr)
			return;
		if (bonus->getName() == speedBonusName)
		{
			if (to_player_1)
				horizontalSpeed_player1 += bonus->getValue();
				
			else
				horizontalSpeed_player2 += bonus->getValue();
		}
		else if (bonus->getName() == jumpBonusName)
		{
			if (to_player_1)
				jumpSpeed_player1 += bonus->getValue();
			else
				jumpSpeed_player2 += bonus->getValue();
		}
		else if (bonus->getName() == massBonusName)
		{
			if (to_player_1)
			{
				for (b2Fixture* it = playersBody[0]->GetFixtureList(); it != 0; it = it->GetNext())
					it->SetDensity(it->GetDensity() + bonus->getValue());
				playersBody[0]->ResetMassData();
			}
			else
			{
				for (b2Fixture* it = playersBody[1]->GetFixtureList(); it != 0; it = it->GetNext())
					it->SetDensity(it->GetDensity() + bonus->getValue());
				playersBody[1]->ResetMassData();
			}
		}
	}

	void takeBonus()
	{
		if (bonus->getName() == speedBonusName)
		{
			if (bonus->isPlayer1_hit())
				horizontalSpeed_player1 -= bonus->getValue();

			else
				horizontalSpeed_player2 -= bonus->getValue();
		}
		else if (bonus->getName() == jumpBonusName)
		{
			if (bonus->isPlayer1_hit())
				jumpSpeed_player1 -= bonus->getValue();
			else
				jumpSpeed_player2 -= bonus->getValue();
		}
		else if (bonus->getName() == massBonusName)
		{
			if (bonus->isPlayer1_hit())
			{
				for (b2Fixture* it = playersBody[0]->GetFixtureList(); it != 0; it = it->GetNext())
					it->SetDensity(it->GetDensity() - bonus->getValue());
				playersBody[0]->ResetMassData();
			}
			else
			{
				for (b2Fixture* it = playersBody[1]->GetFixtureList(); it != 0; it = it->GetNext())
					it->SetDensity(it->GetDensity() - bonus->getValue());
				playersBody[1]->ResetMassData();
			}
		}
	}

private:
	void drawObjects(sf::RenderWindow& W)
	{
		W.draw(board);
		player_1->drawObject(W);
		player_2->drawObject(W);
		ball->drawObject(W);
		if (bonus != nullptr)
			bonus->drawObject(W);
	}
public:


	void drawUpdatePositions(sf::RenderWindow& W)
	{

		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
		{
			b2Vec2 pos = it->GetPosition();
			float angle = it->GetAngle();

			if (it->GetUserData() == (void*)names[0])
			{
				player_1->sprite.setPosition(pos.x * SCALE * Window::getMultiplier_X(), pos.y * SCALE * Window::getMultiplier_Y());
				player_1->sprite.setRotation(angle * DEG);
				//player_1->sprite.setColor(sf::Color::Red);
				player_1->pos = pos;
			}

			if (it->GetUserData() == (void*)names[1])
			{
				player_2->sprite.setPosition(pos.x * SCALE * Window::getMultiplier_X(), pos.y * SCALE * Window::getMultiplier_Y());
				player_2->sprite.setRotation(angle * DEG);
				//player_2->sprite.setColor(sf::Color::Green);
				player_2->pos = pos;
			}

			if (it->GetUserData() == (void*)names[2])
			{
				ball->sprite.setPosition(pos.x * SCALE * Window::getMultiplier_X(), pos.y * SCALE * Window::getMultiplier_Y());
				ball->sprite.setRotation(angle * DEG);
				ball->pos = pos;
			}
		}
		drawObjects(W);
	}

	void resetPosition(bool player_1_ball)
	{
		playersBody[0]->SetTransform(b2Vec2(19.7917f, 37.0617f), 0.f);
		playersBody[1]->SetTransform(b2Vec2(64.4475f, 37.0617f), 0.f);
		if (player_1_ball)
			b->SetTransform(b2Vec2(64.4475f, 10.f), 0.f);
		else
			b->SetTransform(b2Vec2(19.7917f, 10.f), 0.f);
		b->SetLinearVelocity(b2Vec2_zero);
		b->SetAngularVelocity(0.f);

		number_of_pickUp = 0;
	}



	void drawEndMessage(bool player_1_winner, sf::RenderWindow& W)
	{
		std::string content = "Winner is ";
		content += (player_1_winner) ? player_1->getName() : player_2->getName();
		content += "!!!";
		text_end.setString(content);
		sf::Event e;

		for (int i = 0; i < 250; i++)
		{
			while (W.pollEvent(e))
			{
				if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					return;
				}
			}
			W.clear();
			drawUpdatePositions(W);
			W.draw(text_end);
			W.display();
		}
	}

	void drawResult(int result_1, int result_2, sf::RenderWindow& W)
	{
		std::stringstream ss;
		ss << player_1->getName() << " " << result_1 << " : " << result_2 << " " << player_2->getName();
		text.setString(ss.str());
		W.draw(text);
	}
};
