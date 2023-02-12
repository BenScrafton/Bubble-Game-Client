#pragma once
#include <SFML/Graphics.hpp>
#include "Definitions.h"

class Shapes
{
private:
	Shapes()
	{
		//Shapes
		bubbleShape = sf::CircleShape(BUBBLE_SIZE);
		cannonShape = sf::RectangleShape(sf::Vector2f(CANNON_W, CANNON_H));
		wallShape = sf::RectangleShape(sf::Vector2f(1, WINDOW_H));
	}

	sf::CircleShape bubbleShape;
	sf::RectangleShape cannonShape;
	sf::RectangleShape wallShape;

public:
	Shapes(const Shapes&) = delete;

	static Shapes& Get()
	{
		static Shapes static_Instance;
		return static_Instance;
	}
	
	static sf::CircleShape& GetBubbleShape()
	{
		return Get().bubbleShape;
	}

	static sf::RectangleShape& GetCannonShape()
	{
		return Get().cannonShape;
	}

	static sf::RectangleShape& GetWallShape()
	{
		return Get().wallShape;
	}
};

