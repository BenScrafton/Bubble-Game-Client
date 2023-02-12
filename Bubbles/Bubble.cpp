#include "Bubble.h"

void Bubble::Update()
{

}

sf::Color Bubble::GetColour()
{
	return m_colour;
}

void Bubble::SetIsChecked(bool p_isChecked)
{
	m_isChecked = p_isChecked;
}

bool Bubble::GetIsChecked()
{
	return m_isChecked;
}

void Bubble::Render(sf::RenderWindow& window)
{
	Shapes::GetBubbleShape().setPosition(m_pos);
	Shapes::GetBubbleShape().setOrigin(m_origin);
	Shapes::GetBubbleShape().setFillColor(m_colour);

	window.draw(Shapes::GetBubbleShape());
}
