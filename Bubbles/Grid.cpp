#include "Grid.h"

void Grid::Update()
{
	m_scoreTxt.setString(std::to_string(m_score));
	MannageCollisions();

	if(m_score > 20)
	{
		m_gameWon = true;
	}
}

void Grid::CreateGrid()
{
	for (size_t y = 0; y < m_initNumRows; y++)
	{
		for (size_t x = 0; x < m_numColumns; x++)
		{
			int posX = (x + BUBBLE_SIZE) + (x * BUBBLE_SIZE * 2);
			int posY = y * 33 + m_initYOffset;

			if ((y % 2) == 0)
			{
				posX += BUBBLE_SIZE;
			}

			if(!m_isP1)
			{
				posX += m_P2XOffset;
			}

			sf::Vector2f pos = sf::Vector2f(posX, posY);
			sf::Color colour = colours[rng() % 5];
			Bubble* newBubble = new Bubble(pos, sf::Vector2f(BUBBLE_SIZE, BUBBLE_SIZE), 0, colour);

			m_bubbleGrid[x][y] = newBubble;
		}
	}
}

void Grid::MannageCollisions()
{
	for (int y = m_numRows - 1; y >= 0; y--)
	{
		for (int x = 0; x < m_numColumns; x++)
		{
			if(m_bubbleGrid[x][y] == nullptr)
			{
				continue;
			}

			if (CheckCollision(x, y))
			{
				int checkX = x;
				int checkY = y;
				Snap(checkX, checkY);

				//Delete Bubbles with Same Colour
				UnCheckBubbles();
				PopSameColourBubbles(checkX, checkY);

				//Delete Floating Bubbles
				UnCheckBubbles();
				PopFloatingBubbles();

				if(m_snapCount > 5)
				{
					AddRow();
					m_snapCount = 0;
				}

				//Delete Floating Bubbles
				UnCheckBubbles();
				PopFloatingBubbles();

				m_cannonBubble->SetIsActive(false);
				m_cannonBubble->Reset();

				return;
			}
		}
	}
}

bool Grid::CheckCollision(int x, int y)
{
	float distX = abs(m_bubbleGrid[x][y]->GetPos().x - m_cannonBubble->GetPos().x);
	float distY = abs(m_bubbleGrid[x][y]->GetPos().y - m_cannonBubble->GetPos().y);

	float distSquared = (distX * distX) + (distY * distY);
	float compareDist = (BUBBLE_SIZE * 2) * (BUBBLE_SIZE * 2);

	if(distSquared < compareDist)
	{
		return true;
	}
	return false;
}

void Grid::Snap(int& bx, int& by)
{
	int idxY = by;
	int idxX = bx;

	float newX;
	float newY;

	if (m_cannonBubble->GetPos().x < m_bubbleGrid[bx][by]->GetPos().x)
	{
		newX = m_bubbleGrid[bx][by]->GetPos().x - BUBBLE_SIZE;
		if((by % 2) != 0)
		{
			idxX -= 1;
		}
	}
	else
	{
		newX = m_bubbleGrid[bx][by]->GetPos().x + BUBBLE_SIZE;
		if ((by % 2) == 0)
		{
			idxX += 1;
		}
	}

	if (m_cannonBubble->GetPos().y > m_bubbleGrid[bx][by]->GetPos().y + BUBBLE_SIZE / 3)
	{
		//BOTTOM
		newY = m_bubbleGrid[bx][by]->GetPos().y + 33;
		idxY += 1;
	}
	else if(m_cannonBubble->GetPos().y < m_bubbleGrid[bx][by]->GetPos().y - BUBBLE_SIZE / 3)
	{
		//TOP
		newY = m_bubbleGrid[bx][by]->GetPos().y - 33;
		idxY -= 1;
	}
	else
	{
		//SAME
		newY = m_bubbleGrid[bx][by]->GetPos().y;

		if (m_cannonBubble->GetPos().x < m_bubbleGrid[bx][by]->GetPos().x)
		{
			if((by % 2) == 0)
			{
				idxX -= 1;
			}
			newX -= BUBBLE_SIZE;
		}
		else
		{
			if ((by % 2) != 0)
			{
				idxX += 1;
			}
			newX += BUBBLE_SIZE;
		}
	}

	sf::Vector2f pos = sf::Vector2f(newX, newY);

	Bubble* b = new Bubble(pos, sf::Vector2f(BUBBLE_SIZE, BUBBLE_SIZE), 0, m_cannonBubble->GetColour());

	std::cout << "Snapped pos: " << idxX << " " << idxY << std::endl;

	m_bubbleGrid[idxX][idxY] = b;

	bx = idxX;
	by = idxY;

	m_numRowsTaken++;
}

void Grid::CheckCluster(int p_x, int p_y, bool popColour, sf::Color colour)
{
	int checkX;
	int checkY;

	for (size_t y = 0; y < 3; y++)
	{
		for (size_t x = 0; x < 2; x++)
		{
			if ((p_y % 2) == 0)
			{
				if(y != 1)
				{
					checkX = p_x + x;
				}
				else
				{
					checkX = p_x + x + x - 1;
				}
			}
			else
			{
				if (y != 1)
				{
					checkX = p_x + x - 1;
				}
				else
				{
					checkX = p_x + x + x - 1;
				}
			}

			checkY = p_y - 1 + y;

			if(checkX > 13 || checkX < 0 || checkY < 0 || checkY > 24)
			{
				continue;
			}

			if (m_bubbleGrid[checkX][checkY] == nullptr)
			{
				continue;
			}

			if(popColour)
			{
				if(m_bubbleGrid[checkX][checkY]->GetIsChecked())
				{
					continue;
				}

				m_bubbleGrid[checkX][checkY]->SetIsChecked(true);

				if (m_bubbleGrid[checkX][checkY]->GetColour() == m_cannonBubble->GetColour())
				{
					CheckCluster(checkX, checkY, true, m_cannonBubble->GetColour());
					m_clusterIndex.push_back(sf::Vector2f(checkX, checkY));
				}
			}
			else
			{
				if(!m_bubbleGrid[checkX][checkY]->GetIsChecked())
				{
					m_bubbleGrid[checkX][checkY]->SetIsChecked(true);
					CheckCluster(checkX, checkY, false);
				}
			}
		}
	}


}

void Grid::UnCheckBubbles()
{
	for (size_t y = 0; y < m_numRows; y++)
	{
		for (size_t x = 0; x < m_numColumns; x++)
		{
			if (m_bubbleGrid[x][y] == nullptr)
			{
				continue;
			}

			m_bubbleGrid[x][y]->SetIsChecked(false);
		}
	}
}

void Grid::PopBubble(int p_x, int p_y)
{
	Bubble* b = m_bubbleGrid[p_x][p_y];
	delete(b);
	m_bubbleGrid[p_x][p_y] = nullptr;
	m_score++;
}

void Grid::PopSameColourBubbles(int checkX, int checkY)
{
	m_clusterIndex.clear();
	CheckCluster(checkX, checkY, true, m_cannonBubble->GetColour());
	if (m_clusterIndex.size() > 2)
	{
		for (size_t i = 0; i < m_clusterIndex.size(); i++)
		{
			int xIdx = m_clusterIndex[i].x;
			int yIdx = m_clusterIndex[i].y;

			PopBubble(xIdx, yIdx);
		}
	}
	else
	{
		m_snapCount++;
	}
}

void Grid::PopFloatingBubbles()
{
	for (size_t x1 = 0; x1 < m_numColumns; x1++)
	{
		if (m_bubbleGrid[x1][0] == nullptr)
		{
			continue;
		}

		CheckCluster(x1, 0, false);
	}

	for (size_t y = 0; y < m_numRows; y++)
	{
		for (size_t x = 0; x < m_numColumns; x++)
		{
			if (m_bubbleGrid[x][y] == nullptr)
			{
				continue;
			}

			if (!m_bubbleGrid[x][y]->GetIsChecked())
			{
				PopBubble(x, y);
			}
		}
	}
}

void Grid::AddRow()
{
	for (int y = m_numRows - 1; y >= 0; y--)
	{
		for (int x = 0; x < m_numColumns; x++)
		{
			Bubble* b = m_bubbleGrid[x][y];
			m_bubbleGrid[x][y] = nullptr;

			if (b != nullptr) {
				sf::Vector2f pos = b->GetPos();

				if ((y % 2) == 0)
				{
					pos.x -= BUBBLE_SIZE;
				}
				else
				{
					pos.x += BUBBLE_SIZE;
				}
				pos.y += 33;

				b->SetPos(pos);

				m_bubbleGrid[x][y + 1] = b;
			}
		}
	}
	
	int y = 0;
	for (size_t x = 0; x < m_numColumns; x++)
	{
		int posX = (x + BUBBLE_SIZE) + (x * BUBBLE_SIZE * 2);
		int posY = y * 33 + m_initYOffset;

		if ((y % 2) == 0)
		{
			posX += BUBBLE_SIZE;
		}

		if (!m_isP1)
		{
			posX += m_P2XOffset;
		}

		sf::Vector2f pos = sf::Vector2f(posX, posY);
		sf::Color colour = colours[rng() % 5];
		Bubble* newBubble = new Bubble(pos, sf::Vector2f(BUBBLE_SIZE, BUBBLE_SIZE), 0, colour);

		m_bubbleGrid[x][y] = newBubble;
	}
}

void Grid::SetRandSeed(int p_seed)
{
	//srand(p_seed);
}

int Grid::GetScore()
{
	return m_score;
}

bool Grid::GetGameWon()
{
	return m_gameWon;
}

void Grid::Render(sf::RenderWindow& window)
{
	for (size_t y = 0; y < m_numRows; y++)
	{
		for (size_t x = 0; x < m_numColumns; x++)
		{
			if (m_bubbleGrid[x][y] == nullptr)
			{
				continue;
			}

			m_bubbleGrid[x][y]->Render(window);
		}
	}
}
