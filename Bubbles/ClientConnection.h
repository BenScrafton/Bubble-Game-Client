#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "Message.h"
#include "Cannon.h"

class ClientConnection
{
private:
	sf::TcpSocket m_serverSocket;
	sf::IpAddress m_ip;
	int m_port;

	Cannon* m_cannonP1;
	Cannon* m_cannonP2;


	bool m_isP1sGame{ false };
	bool m_isRegistered{ false };
	bool m_isClosed{ false };
	bool m_isGameReady{ false };

	int m_playerIndex{0};
	int m_randSeed{0};

public:

	ClientConnection(Cannon* p_cP1, Cannon* p_cP2)
	{
		m_cannonP1 = p_cP1;
		m_cannonP2 = p_cP2;
	}

	void Run();
	void Close();
	void DiscoverNetwork();
	void ConnectToServer();
	void ReceiveMessage();
	void SendMessage();

	bool GetIsGameReady();
	bool GetIsRegistered();

	unsigned int GetRandSeed();
};

