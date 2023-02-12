#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class GameObject
{
protected:
	sf::Vector2f m_pos;
	sf::Vector2f m_origin;
	float m_rotation;

public:
	GameObject(sf::Vector2f p_pos, sf::Vector2f p_origin, float p_rotation)
	{
		m_pos = p_pos;
		m_origin = p_origin;
		m_rotation = p_rotation;
	}

	virtual void Update() {};

	sf::Vector2f GetPos();
	float GetRotation();

	void SetPos(sf::Vector2f p_pos);
	void SetRotation(float p_rotation);

	virtual void Render(sf::RenderWindow& window) {};
};

