#pragma once
#include <iostream>

#include "GameObject.h"
#include "Definitions.h"
#include "Shapes.h"
#include "CannonBubble.h"
#include "Message.h"

class Cannon : public GameObject
{
private:	
	bool m_isP1{ false };
	bool m_isActive{ false };

	CannonBubble* m_cannonBubble;

	sf::Uint8 m_msgType{0};

public:

	Cannon(sf::Vector2f p_pos, sf::Vector2f p_origin, float p_rotation, bool p_isP1, CannonBubble* p_cannonBubble) : GameObject(p_pos, p_origin, p_rotation)
	{
		m_isP1 = p_isP1;
		m_cannonBubble = p_cannonBubble;
		m_cannonBubble->SetIsP1(m_isP1);
	}

	void Update() override;
	void ProcessInput();
	void ProcessRotInput();
	void ProcessFireInput();
	void Rotate(int p_dir); //-1 left 1 right
	void Fire(bool p_fromServer);

	bool GetIsActive();
	
	sf::Uint8 GetMsgType();

	void SetIsActive(bool p_isActive);
	void SetMsgType(sf::Uint8 p_type);

	CannonBubble* GetCannonBubble();

	void Render(sf::RenderWindow& window) override;
};

