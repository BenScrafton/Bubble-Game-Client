#pragma once
#include "GameObject.h"
#include "Bubble.h"
#include "CannonBubble.h"
#include <random>

class Grid :
    public GameObject
{
private:
    Bubble* m_bubbleGrid[14][25];

    int m_numColumns{ 14 };
    int m_numRows{ 25 };
    int m_initNumRows{ 5 };
    int m_numRowsTaken{ 0 };
    int m_initYOffset{ 100 };
    int m_P2XOffset{ WINDOW_W / 2 };
    int m_snapCount{ 0 };

    int m_score{ 0 };

    bool m_isP1{false};
    bool m_gameWon{ false };

    std::vector<sf::Vector2f> m_clusterIndex;
    CannonBubble* m_cannonBubble;

    sf::Text m_scoreTxt;

    std::mt19937 rng{};
public:
    Grid(sf::Vector2f p_pos, sf::Vector2f p_origin, float p_rotation, bool p_isP1, CannonBubble* p_cannonBubble,unsigned int seed):GameObject(p_pos, p_origin, p_rotation)
    {
        m_isP1 = p_isP1;
        m_cannonBubble = p_cannonBubble;
        rng.seed(seed);
        CreateGrid();
    }

    void Update() override;
    void CreateGrid();
    void MannageCollisions();
    bool CheckCollision(int x, int y);
    void Snap(int& bx, int& by);
    void CheckCluster(int p_x, int p_y, bool popColour, sf::Color colour = sf::Color(0,0,0));
    void UnCheckBubbles();
    void PopBubble(int p_x, int p_y);
    void PopSameColourBubbles(int checkX, int checkY);
    void PopFloatingBubbles();
    void AddRow();

    int GetScore();
    bool GetGameWon();

    void SetRandSeed(int p_seed);

    void Render(sf::RenderWindow& window) override;
};

