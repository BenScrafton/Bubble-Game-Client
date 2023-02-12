#include "CannonBubble.h"
#include <random>

void CannonBubble::Update()
{
	if(m_isActive)
	{
		Move();
	}
}

void CannonBubble::Move()
{
	m_pos.x += m_dir.x;
	m_pos.y += m_dir.y;

	//Bounce Off Wall
	if (m_isP1)
	{
		if (m_pos.x < BUBBLE_SIZE || m_pos.x > WINDOW_W / 2 - BUBBLE_SIZE)
		{
			m_dir.x = -m_dir.x;
		}
	}
	else
	{
		if (m_pos.x < WINDOW_W / 2 + BUBBLE_SIZE || m_pos.x > WINDOW_W - BUBBLE_SIZE)
		{
			m_dir.x = -m_dir.x;
		}
	}
}

void CannonBubble::Reset()
{
	m_pos = m_startPos;
	int index = rng() % 5;
	sf::Color c = colours[index];
	m_colour = c;
}

void CannonBubble::SetDir(sf::Vector2f p_dir)
{
	m_dir = p_dir;
}

void CannonBubble::SetIsActive(bool p_isActive)
{
	m_isActive = p_isActive;
}

void CannonBubble::SetIsP1(bool p_isP1)
{
	m_isP1 = p_isP1;
}

void CannonBubble::SetRandSeed(int p_seed)
{
	rng.seed(p_seed);
}

void CannonBubble::SetIsP1sGame(bool p_isP1sGame)
{
	m_isP1sGame = p_isP1sGame;
}

bool CannonBubble::GetIsActive()
{
	return m_isActive;
}

