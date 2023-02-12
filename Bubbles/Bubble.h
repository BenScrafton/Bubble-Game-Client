#pragma once
#include <iostream>

#include "GameObject.h"
#include "Definitions.h"
#include "Shapes.h"

class Bubble :
    public GameObject
{
protected:
    sf::Color m_colour;
    bool m_isChecked{ false };
public:

    Bubble(sf::Vector2f p_pos, sf::Vector2f p_origin, float p_rotation, sf::Color p_colour):GameObject(p_pos, p_origin, p_rotation)
    {
        m_colour = p_colour;
    }
    void Update() override;
    sf::Color GetColour();

    void SetIsChecked(bool p_isChecked);

    bool GetIsChecked();
    void Render(sf::RenderWindow& window) override;
};

