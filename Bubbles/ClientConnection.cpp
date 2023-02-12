#include "ClientConnection.h"

sf::Packet& operator <<(sf::Packet& packet, Message& msg) // send
{
    return packet << msg.msg << msg.type << msg.myIndex << msg.rotate << msg.seed;
}

sf::Packet& operator >>(sf::Packet& packet, Message& msg) // receive
{
    return packet >> msg.msg >> msg.type >> msg.myIndex >> msg.rotate >> msg.seed;
}

void ClientConnection::Run()
{
    DiscoverNetwork();
    ConnectToServer();

    try
    {
        std::thread receiverThread(&ClientConnection::ReceiveMessage, this);
                
        while (true && !m_isClosed)
        {
            SendMessage();
        }

        if (receiverThread.joinable()) {
            receiverThread.join();
        }
        
    }
    catch (const std::exception&)
    {
        std::cout << "Catch 1" << std::endl;
    }
}

void ClientConnection::Close()
{
    m_isClosed = true;
}

void ClientConnection::DiscoverNetwork()
{
    sf::UdpSocket udpConnection;
    char msg[7] = "Client";

    if (udpConnection.bind(54000) != sf::Socket::Done)
    {
        std::cout << "Error" << std::endl;
    }

    if (udpConnection.send(msg, sizeof(msg), sf::IpAddress::Broadcast, 53000))
    {
        std::cout << "Error\n";
    }

    bool networkFound = false;
    char msgReceived[7];
    size_t received;

    sf::IpAddress remoteIp;
    unsigned short remotePort;

    while (!networkFound)
    {
        if (udpConnection.receive(msgReceived, sizeof(msgReceived), received, remoteIp, remotePort) == sf::Socket::Done)
        {
            std::cout << "msg RECEIVED!" << std::endl;

            if (strcmp(msgReceived, "Server") == 0)
            {
                m_ip = remoteIp;

                std::cout << "network found" << std::endl;
                networkFound = true;
            }
        }
    }
}

void ClientConnection::ConnectToServer()
{
    if (m_serverSocket.connect(m_ip, 53000) != sf::Socket::Done)
    {
        std::cout << "Could not connect to server\n";
    }
}

void ClientConnection::ReceiveMessage()
{

    try
    {
        // Set server socket to not block otherwise wont be able to join
        m_serverSocket.setBlocking(false);

        while (true && !m_isClosed)
        {
            sf::Packet packet;
            Message m;

            if (m_serverSocket.receive(packet) == sf::Socket::Disconnected)
            {
                return;
            }

            if (packet >> m)
            {
                //REGSITER----------------------------------------
                if (m.type == MSG_REGISTER)
                {
                    std::cout << "REGISTER" << std::endl;

                    if (m.myIndex == 0) //isPlayer1
                    {
                        m_isP1sGame = true;
                        m_playerIndex = 0;
                        m_cannonP1->SetIsActive(true);
                        m_cannonP1->GetCannonBubble()->SetIsP1sGame(true);
                        m_cannonP2->GetCannonBubble()->SetIsP1sGame(true);
                    }
                    else //isPlayer2
                    {
                        m_isP1sGame = false;
                        m_playerIndex = 1;
                        m_cannonP2->SetIsActive(true);
                        m_cannonP2->GetCannonBubble()->SetIsP1sGame(false);
                        m_cannonP1->GetCannonBubble()->SetIsP1sGame(false);
                    }

                    m_cannonP1->GetCannonBubble()->SetRandSeed(m.seed);
                    m_cannonP2->GetCannonBubble()->SetRandSeed(m.seed);

                    m_randSeed = m.seed;
                    m_isRegistered = true;
                    //END OF REGISTER---------------------------------
                }

                //SET GAME TO READY-------------------------------
                if (m.type == MSG_READY)
                {
                    m_isGameReady = true;
                }
                //END OF SET GAME TO READY------------------------

                //MANAGE INPUT MESSAGES---------------------------------
                if (m_isP1sGame)//Running Player1's game
                {
                    //Player1's game receives messages from Player2

                    if (m.type & MSG_ROTATE)
                    {
                        m_cannonP2->SetRotation(m.rotate);
                    }

                    if (m.type & MSG_FIRE)
                    {
                        m_cannonP2->Fire(true);
                    }
                }
                else //Running Player2's game
                {
                    //Player2's game receives messages from Player1

                    if (m.type & MSG_ROTATE)
                    {
                        m_cannonP1->SetRotation(m.rotate);
                    }

                    if (m.type & MSG_FIRE)
                    {
                        m_cannonP1->Fire(true);
                    }
                }
                //END OF MANAGE INPUT MESSAGES--------------------------
            }
        }
    }
    catch (const std::exception&)
    {
        std::cout << "Exception " << std::endl;
    }
}

void ClientConnection::SendMessage()
{
    try
    {
        sf::Packet packet;
        Message m;

        if (m_isP1sGame)//Running Player1's game
        {
            //Player1's game sends messages to Player2's game

            if (m_cannonP1->GetMsgType() == MSG_NONE)
            {
                return;
            }

            m.myIndex = m_playerIndex;
            m.type = m_cannonP1->GetMsgType();
            m.rotate = m_cannonP1->GetRotation();
        }
        else//Running Player2's game
        {
            //Player2's game sends messages to Player1's game

            if (m_cannonP2->GetMsgType() == MSG_NONE)
            {
                return;
            }

            m.myIndex = m_playerIndex;
            m.type = m_cannonP2->GetMsgType();
            m.rotate = m_cannonP2->GetRotation();
        }

        packet << m;

        if (m_serverSocket.send(packet) != sf::Socket::Done)//Send the Message to the Server
        {
            std::cout << "ERROR: Failed to send Message\n";
        }
        else
        {
            std::cout << "Sent Message: seed " << m.seed << "," << (int)m.type << "\n";
        }

        m_cannonP1->SetMsgType(MSG_NONE);
        m_cannonP2->SetMsgType(MSG_NONE);
    }
    catch (const std::exception&)
    {
        std::cout << "IN send thread exception " << std::endl;
    }
 
}

bool ClientConnection::GetIsGameReady()
{
    return m_isGameReady;
}

bool ClientConnection::GetIsRegistered()
{
    return m_isRegistered;
}

unsigned int ClientConnection::GetRandSeed()
{
    return m_randSeed;
}
