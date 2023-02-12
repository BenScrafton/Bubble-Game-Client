#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>

#include "Definitions.h"
#include "GameObject.h"
#include "Cannon.h"
#include "Grid.h"
#include "ClientConnection.h"

std::vector<GameObject*> gameObjects;


int main(int argc, const char* argv[])
{

    try
    {
        std::cout << "Started" << std::endl;
        //INITIALISE----------------------------------------------------------------------------------------------------------------------------------

        //RENDER WINDOW SETUP--------------------------------------------------
        sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Bubble");

        
        window.setFramerateLimit(60);
        //END OF RENDER WINDOW SETUP-------------------------------------------

        //CANNONS SETUP-------------------------------------------------------------------------------------------------------------
        //P1
        sf::Vector2f p1_pos(WINDOW_W / 4, (WINDOW_H));
        sf::Vector2f p1_origin(CANNON_W / 2, CANNON_H);

        CannonBubble* cbP1 = new CannonBubble(p1_pos, sf::Vector2f(BUBBLE_SIZE, BUBBLE_SIZE), 0, sf::Color(colours[rand() % 5]));
        Cannon* cannonP1 = new Cannon(p1_pos, p1_origin, 0, true, cbP1);

        //P2
        sf::Vector2f p2_pos(3 * WINDOW_W / 4, (WINDOW_H));
        sf::Vector2f p2_origin(CANNON_W / 2, CANNON_H);

        CannonBubble* cbP2 = new CannonBubble(p2_pos, sf::Vector2f(BUBBLE_SIZE, BUBBLE_SIZE), 0, sf::Color(colours[rand() % 5]));
        Cannon* cannonP2 = new Cannon(p2_pos, p2_origin, 0, false, cbP2);

        gameObjects.push_back(cannonP1);
        gameObjects.push_back(cannonP2);
        gameObjects.push_back(cbP1);
        gameObjects.push_back(cbP2);
        //END OF CANNONS SETUP------------------------------------------------------------------------------------------------------

        //CONNECT TO NETWORK----------------------------------------------------------------

        ClientConnection* clientConnection = new ClientConnection(cannonP1, cannonP2);

        std::thread clientThread(&ClientConnection::Run, clientConnection);

        while (!clientConnection->GetIsRegistered())
        {
            //Wait until Registered
        }

        //END OF CONNECT TO NETWORK---------------------------------------------------------

        //SET RAND SEED---------------------------------------------------------------------
        int seed = clientConnection->GetRandSeed();
        
        std::cout << "Seed: " << seed << std::endl;
        srand(seed);
        //END OF SET RAND SEED--------------------------------------------------------------

        //GRID SETUP----------------------------------------------------------------------------------
        Grid* gridP1 = new Grid(sf::Vector2f(0, 0), sf::Vector2f(0, 0), 0, true, cbP1, seed);
        Grid* gridP2 = new Grid(sf::Vector2f(0, 0), sf::Vector2f(0, 0), 0, false, cbP2, seed);

        gameObjects.push_back(gridP1);
        gameObjects.push_back(gridP2);
        //END OF GRID SETUP---------------------------------------------------------------------------

        //WALL SETUP----------------------------------------
        sf::RectangleShape wall(sf::Vector2f(1, WINDOW_H));
        wall.setPosition(WINDOW_W / 2, 0);
        //END OF WALL SETUP---------------------------------

        //TEXT SETUP----------------------------------------
        //Load font
        sf::Font font;
        if (!font.loadFromFile("PressStart2P.ttf"))
        {
            return 1;
        }

        //Win message
        sf::Text winTxt("P1-WINS", font);
        winTxt.setCharacterSize(100);
        winTxt.setPosition(WINDOW_H / 2, WINDOW_H / 2 - 100);

        //Wait message
        sf::Text waitTxt("waiting for P2", font);
        waitTxt.setPosition(10, 10);

        //P1 score
        sf::Text P1scoreTxt("0", font);
        P1scoreTxt.setCharacterSize(30);
        P1scoreTxt.setPosition(10, 10);

        sf::Text playerTxt("0", font);
        P1scoreTxt.setCharacterSize(30);
        P1scoreTxt.setPosition(10, 10);

        //P2 score
        sf::Text P2scoreTxt("0", font);
        P2scoreTxt.setCharacterSize(30);
        P2scoreTxt.setPosition(WINDOW_H + 10, 10);
        //END OF TEXT SETUP---------------------------------

        //END OF INITIALISE---------------------------------------------------------------------------------------------------------------------------

        while (window.isOpen())
        {
            sf::Event e;
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed) {
                    // Notify client to close 
                    clientConnection->Close();
                    
                    // Join thread to close gracefully
                    if (clientThread.joinable()) {
                        clientThread.join();
                    }

                    window.close();
                }
            }

            //UPDATE-----------------------------------------------------------------------------------
            if (clientConnection->GetIsGameReady() && !gridP1->GetGameWon() && !gridP2->GetGameWon())
            {
                //Update GameObjects
                for (GameObject* g : gameObjects)
                {
                    g->Update();
                }

                //Update Text
                P1scoreTxt.setString(std::to_string(gridP1->GetScore()));
                P2scoreTxt.setString(std::to_string(gridP2->GetScore()));
            }
            //END OF UPDATE----------------------------------------------------------------------------

            //RENDER---------------------------------------------------------
            window.clear();

            //Render GameObjects
            for (GameObject* g : gameObjects)
            {
                g->Render(window);
            }

            //Render txt
            window.draw(wall);
            if (clientConnection->GetIsGameReady())
            {
                window.draw(P1scoreTxt);
                window.draw(P2scoreTxt);
            }
            else
            {
                window.draw(waitTxt);
            }

            if (gridP1->GetGameWon())
            {
                winTxt.setString("P1-WINS");
                window.draw(winTxt);
            }
            if (gridP2->GetGameWon())
            {
                winTxt.setString("P2-WINS");
                window.draw(winTxt);
            }

            if (clientConnection->GetIsGameReady())
            {
                if (cannonP1->GetIsActive())
                {
                    playerTxt.setString("You are P1");
                    playerTxt.setPosition(WINDOW_H - playerTxt.getGlobalBounds().width - 10, 10);
                }
                else
                {
                    playerTxt.setString("You are P2");
                    playerTxt.setPosition(WINDOW_W - playerTxt.getGlobalBounds().width - 10, 10);
                }

                window.draw(playerTxt);
            }

            window.display();
            //END OF RENDER--------------------------------------------------
        }
    }
    catch (const std::exception& e)
    {
            
    }
    
    return 0;
}
