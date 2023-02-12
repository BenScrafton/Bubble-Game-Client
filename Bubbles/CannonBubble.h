#pragma once
#include "Bubble.h"
#include <random>


class CannonBubble :
    public Bubble
{
private:
    sf::Vector2f m_startPos;
    sf::Vector2f m_dir;

    float m_speed{ 10.0f };

    bool m_isActive{ false };
    bool m_isP1{ false };
    bool m_isP1sGame{ false }; // signifies if this game is P1's game

    std::mt19937 rng{};

public:

    CannonBubble(sf::Vector2f p_startPos, sf::Vector2f p_origin, float p_rotation, sf::Color p_colour):Bubble(p_startPos, p_origin, p_rotation, p_colour)
    {
        m_startPos = p_startPos;
    }

    void Update() override;
    void Move();
    void Reset();

    void SetDir(sf::Vector2f p_dir);
    void SetIsActive(bool p_isActive);
    void SetIsP1(bool p_isP1);
    void SetRandSeed(int p_seed);
    void SetIsP1sGame(bool p_isP1sGame);

    bool GetIsActive();

    //Don't need to override the Render() as Bubble would have the same Render function 
};

