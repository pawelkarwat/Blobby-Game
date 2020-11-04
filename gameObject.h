#pragma once

#include <SFML/Graphics.hpp>

class gameObject
{
public:
	sf::Sprite sprite;

	b2Vec2 vel;
	b2Vec2 pos;
protected:
	virtual void loadTexture() = 0;
public:
	virtual void drawObject(sf::RenderWindow& W) = 0;
	virtual std::string getName() = 0;
	//virtual bool is_a_collision(float x, float y) = 0;
};
