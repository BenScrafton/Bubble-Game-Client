#include "Cannon.h"

void Cannon::Update()
{
    if(m_isActive)
    {
        ProcessInput();
    }
}

void Cannon::ProcessInput()
{
    ProcessRotInput();
    ProcessFireInput();
}

void Cannon::ProcessRotInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (m_rotation > MIN_ANGLE + 1 || m_rotation < MAX_ANGLE + 1))
    {
        Rotate(-1);
        m_msgType = MSG_ROTATE;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (m_rotation > MIN_ANGLE - 1 || m_rotation < MAX_ANGLE - 1))
    {
        Rotate(1);
        m_msgType = MSG_ROTATE;
    }
}

void Cannon::ProcessFireInput()
{

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        Fire(false);    
    }
}

void Cannon::Rotate(int p_dir)
{
    m_rotation += p_dir;
}

void Cannon::Fire(bool p_fromServer)
{
    if(!m_cannonBubble->GetIsActive())
    {
        float dx = -cos((m_rotation + 90) * M_PI / 180) * VELOCITY;
        float dy = -sin((m_rotation + 90) * M_PI / 180) * VELOCITY;

        m_cannonBubble->SetDir(sf::Vector2f(dx, dy));
        m_cannonBubble->SetIsActive(true);

        if(!p_fromServer)
        {
            m_msgType = MSG_FIRE | MSG_ROTATE;
        }
    }
}

bool Cannon::GetIsActive()
{
    return m_isActive;
}

sf::Uint8 Cannon::GetMsgType()
{
    return m_msgType;
}

void Cannon::SetIsActive(bool p_isActive)
{
    m_isActive = p_isActive;
}

void Cannon::SetMsgType(sf::Uint8 p_type)
{
    m_msgType = p_type;
}

CannonBubble* Cannon::GetCannonBubble()
{
    return m_cannonBubble;
}

void Cannon::Render(sf::RenderWindow& window)
{
    Shapes::GetCannonShape().setOrigin(m_origin);
    Shapes::GetCannonShape().setPosition(m_pos);
    Shapes::GetCannonShape().setRotation(m_rotation);

	window.draw(Shapes::GetCannonShape());
}
