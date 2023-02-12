#pragma once
#include <SFML/Network.hpp>
#include <vector>

#define MSG_NONE 0
#define MSG_REGISTER 1
#define MSG_ROTATE 2
#define MSG_FIRE 4
#define MSG_READY 8

struct Message
{
	sf::Uint8 type{ MSG_NONE };
	int seed;
	int myIndex{ 0 };
	float rotate;
	std::string msg;
};
