#include "GameObject.h"

sf::Vector2f GameObject::GetPos()
{
    return m_pos;
}

float GameObject::GetRotation()
{
    return m_rotation;
}

void GameObject::SetPos(sf::Vector2f p_pos)
{
    m_pos = p_pos;
}

void GameObject::SetRotation(float p_rotation)
{
    m_rotation = p_rotation;
}
