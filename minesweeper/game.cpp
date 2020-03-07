#include "game.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <utility>

#define wh windowHandler

Game::Game() : wh(), timer()
{
	this->gameState = NEW_GAME;
	this->sizeRow = EASY_ROW;
	this->sizeCol = EASY_COL;
	this->mines = EASY_MINES;
	this->minesRemaining = mines;
	
	srand(time(NULL));
}

Game::~Game()
{
	DeleteBoard();
}

void Game::DeleteBoard()
{
	for (int row = 0; row < board.size(); row++)
	{
		for (int col = 0; col < board[0].size(); col++)
		{
			delete board[row][col];
		}
		board.clear();
	}
}

void Game::ResetGame(const Level level)
{
	DeleteBoard();
	gameState = NEW_GAME;
	timer.Reset();

	switch (level)
	{
	case EASY:
		wh.Resize(EASY_WIN_WIDTH, EASY_WIN_HEIGHT);
		this->sizeRow = EASY_ROW;
		this->sizeCol = EASY_COL;
		this->mines = EASY_MINES;
		break;
	case MEDIUM:
		wh.Resize(MEDIUM_WIN_WIDTH, MEDIUM_WIN_HEIGHT);
		this->sizeRow = MEDIUM_ROW;
		this->sizeCol = MEDIUM_COL;
		this->mines = MEDIUM_MINES;
		break;
	case HARD:
		wh.Resize(HARD_WIN_WIDTH, HARD_WIN_HEIGHT);
		this->sizeRow = HARD_ROW;
		this->sizeCol = HARD_COL;
		this->mines = HARD_MINES;
		break;
	}

	this->minesRemaining = mines;
}

void Game::GenerateBoard(const int& clickedRow, const int& clickedCol)
{
	// create temporary board
	char** tempBoard;
	tempBoard = new char*[sizeRow];
	for (int i = 0; i < sizeRow; i++)
	{
		tempBoard[i] = new char[sizeCol];
	}

	// keep generating new configurations until a sufficient one is produced
	while (true)
	{
		// fill board with empty characters
		for (int row = 0; row < sizeRow; row++)
		{
			for (int col = 0; col < sizeCol; col++)
			{
				tempBoard[row][col] = ' ';
			}
		}

		// add mines randomly to board
		for (int i = 0; i < mines; i++)
		{
			while (true)
			{
				int row = rand() % sizeRow;
				int col = rand() % sizeCol;
				if (tempBoard[row][col] != '*')
				{
					tempBoard[row][col] = '*';
					break;
				}
			}
		}

		// count adjacent mines for all non-mine tiles
		for (int row = 0; row < sizeRow; row++)
		{
			for (int col = 0; col < sizeCol; col++)
			{
				if (tempBoard[row][col] != '*')
				{
					int mineCount = 0;
					CountAdjacentMines(row - 1, col, tempBoard, mineCount);
					CountAdjacentMines(row - 1, col + 1, tempBoard, mineCount);
					CountAdjacentMines(row, col + 1, tempBoard, mineCount);
					CountAdjacentMines(row + 1, col + 1, tempBoard, mineCount);
					CountAdjacentMines(row + 1, col, tempBoard, mineCount);
					CountAdjacentMines(row + 1, col - 1, tempBoard, mineCount);
					CountAdjacentMines(row, col - 1, tempBoard, mineCount);
					CountAdjacentMines(row - 1, col - 1, tempBoard, mineCount);
					tempBoard[row][col] = '0' + mineCount;
				}
			}
		}

		if (tempBoard[clickedRow][clickedCol] == '0')
		{
			// create real board from temp board
			for (int row = 0; row < sizeRow; row++)
			{
				board.push_back(std::vector<Tile*>());
				for (int col = 0; col < sizeCol; col++)
				{
					board[row].push_back(new Tile(tempBoard[row][col]));
				}
			}

			RehideBoard();
			break;
		}
	}

	delete[] tempBoard;
}

void Game::GameLoop()
{
	while (wh.window.isOpen())
	{
		HandleInput();
		Update();
		Draw();
	}
}

void Game::HandleInput()
{
	sf::Event evnt;
	sf::Vector2i mousePos;

	while (wh.window.pollEvent(evnt))
	{
		mousePos = sf::Mouse::getPosition(wh.window);
		int x = mousePos.x / TILE_SIZE - 1;
		int y = mousePos.y / TILE_SIZE - 3;

		if (evnt.type == sf::Event::Closed)
		{
			wh.window.close();
		}
		if (evnt.type == sf::Event::MouseButtonPressed)
		{
			// For Tiles
			if ((x >= 0 && x < sizeCol) && (y >= 0 && y < sizeRow))
			{
				// Chording
				if (evnt.mouseButton.button == sf::Mouse::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				{
					if (gameState == ACTIVE)
					{
						if (board[y][x]->IsRevealed())
						{
							int adjacentFlags = 0;
							CountAdjacentFlags(y - 1, x, adjacentFlags);
							CountAdjacentFlags(y - 1, x + 1, adjacentFlags);
							CountAdjacentFlags(y, x + 1, adjacentFlags);
							CountAdjacentFlags(y + 1, x + 1, adjacentFlags);
							CountAdjacentFlags(y + 1, x, adjacentFlags);
							CountAdjacentFlags(y + 1, x - 1, adjacentFlags);
							CountAdjacentFlags(y, x - 1, adjacentFlags);
							CountAdjacentFlags(y - 1, x - 1, adjacentFlags);
							
							if (adjacentFlags == board[y][x]->GetVal() - '0')
							{
								RevealAdjacentTiles(y - 1, x);
								RevealAdjacentTiles(y - 1, x + 1);
								RevealAdjacentTiles(y, x + 1);
								RevealAdjacentTiles(y + 1, x + 1);
								RevealAdjacentTiles(y + 1, x);
								RevealAdjacentTiles(y + 1, x - 1);
								RevealAdjacentTiles(y, x - 1);
								RevealAdjacentTiles(y - 1, x - 1);
							}
						}
					}
				}
				// Reveal Tile
				else if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					switch (gameState)
					{
					case NEW_GAME:
						GenerateBoard(y, x);
						gameState = ACTIVE;
						timer.Start();
					case ACTIVE:
						RevealTile(y, x);
						break;
					}
				}
				// Flag Tile
				else if (evnt.mouseButton.button == sf::Mouse::Right)
				{;
					if (gameState == ACTIVE)
					{
						FlagTile(y, x);
					}
				}
			}
			// For  Level Buttons
			else
			{
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					char buttonClicked = wh.LevelButtonClicked(mousePos.x, mousePos.y);

					switch (buttonClicked)
					{
					case 'e':
						ResetGame(EASY);
						break;
					case 'm':
						ResetGame(MEDIUM);
						break;
					case 'h':
						ResetGame(HARD);
						break;
					}
				}
			}
		}
	}
}

void Game::Update()
{
	if (gameState != NEW_GAME)
	{
		if (CheckIfWon())
		{
			gameState = WIN;
		}

		if (gameState == ACTIVE)
		{
			timer.Update();
			CountMinesRemaining();
		}
	}
}

void Game::Draw()
{
	wh.window.clear(sf::Color(204, 204, 204));
	wh.window.setView(wh.view);

	wh.DrawTimer(timer.GetElapsedTime());
	wh.DrawMineCounter(minesRemaining);
	wh.DrawLevelButtons();

	if (gameState == LOSE)
	{
		wh.DrawWinLoseText("YOU LOSE!");
	}
	else if (gameState == WIN)
	{
		wh.DrawWinLoseText("YOU WIN!");
	}

	for (int row = 0; row < sizeRow; row++)
	{
		for (int col = 0; col < sizeCol; col++)
		{
			if (gameState != NEW_GAME)
			{
				if (board[row][col]->IsRevealed())
				{
					wh.DrawTiles(col, row, board[row][col]->GetVal());
				}
				else if (board[row][col]->IsFlagged())
				{
					wh.DrawTiles(col, row, 'F');
				}
				else
				{
					wh.DrawTiles(col, row, 'U');
				}
			}
			else
			{
				wh.DrawTiles(col, row, 'U');
			}
		}
	}

	wh.window.display();
}

void Game::CountAdjacentMines(const int& row, const int& col, char** tempBoard, int& mineCount)
{
	if ((row >= 0 && row < sizeRow) && (col >= 0 && col < sizeCol))
	{
		if (tempBoard[row][col] == '*')
		{
			mineCount++;
		}
	}
}

void Game::RevealTile(const int& row, const int& col)
{
	if ((row >= 0 && row < sizeRow) && (col >= 0 && col < sizeCol))
	{
		if ((!board[row][col]->IsFlagged()) && (!board[row][col]->IsRevealed()))
		{
			// Lose if Mine
			if (board[row][col]->GetVal() == '*')
			{
				gameState = LOSE;
				board[row][col]->SetVal('!');
				RevealBoard();
			}
			// Recursive if blank
			else if (board[row][col]->GetVal() == '0')
			{
				board[row][col]->Reveal();
				RevealTile(row - 1, col);
				RevealTile(row - 1, col + 1);
				RevealTile(row, col + 1);
				RevealTile(row + 1, col + 1);
				RevealTile(row + 1, col);
				RevealTile(row + 1, col - 1);
				RevealTile(row, col - 1);
				RevealTile(row - 1, col - 1);
			}
			else
			{
				board[row][col]->Reveal();
			}
		}
	}
}

void Game::FlagTile(const int& row, const int& col)
{
	if (!board[row][col]->IsRevealed())
	{
		if (board[row][col]->IsFlagged())
		{
			board[row][col]->ClearFlag();
		}
		else
		{
			board[row][col]->Flag();
		}
	}
}

void Game::RevealBoard()
{
	for (int row = 0; row < sizeRow; row++)
	{
		for (int col = 0; col < sizeCol; col++)
		{
			if (!board[row][col]->IsRevealed())
			{
				board[row][col]->Reveal();
			}
		}
	}
}

bool Game::CheckIfWon()
{
	int unrevealedTiles = 0;

	for (int row = 0; row < sizeRow; row++)
	{
		for (int col = 0; col < sizeCol; col++)
		{
			if ((!board[row][col]->IsRevealed()))
			{
				unrevealedTiles++;
			}
		}
	}

	return unrevealedTiles == mines;
}

void Game::CountMinesRemaining()
{
	int flagCount = 0;

	for (int row = 0; row < sizeRow; row++)
	{
		for (int col = 0; col < sizeCol; col++)
		{
			if (board[row][col]->IsFlagged())
			{
				flagCount++;
			}
		}
	}

	minesRemaining = mines - flagCount;
}

void Game::CountAdjacentUnrevealedTiles(const int& row, const int& col, int& unrevealedTiles) 
{
	if ((row >= 0 && row < sizeRow) && (col >= 0 && col < sizeCol))
	{
		if (!board[row][col]->IsRevealed())
		{
			unrevealedTiles++;
		}
	}
}

void Game::CountAdjacentFlags(const int& row, const int& col, int& adjacentFlags)
{
	if ((row >= 0 && row < sizeRow) && (col >= 0 && col < sizeCol))
	{
		if (board[row][col]->IsFlagged())
		{
			adjacentFlags++;
		}
	}
}

void Game::FlagAdjacentTiles(const int& row, const int& col)
{
	if ((row >= 0 && row < sizeRow) && (col >= 0 && col < sizeCol))
	{
		if ((!board[row][col]->IsFlagged()) && (!board[row][col]->IsRevealed()))
		{
			FlagTile(row, col);
		}
	}
}

void Game::RevealAdjacentTiles(const int& row, const int& col)
{
	if ((row >= 0 && row < sizeRow) && (col >= 0 && col < sizeCol))
	{
		if ((!board[row][col]->IsRevealed()) && (!board[row][col]->IsFlagged()))
		{
			RevealTile(row, col);
		}
	}
}

void Game::RehideBoard()
{
	for (int row = 0; row < sizeRow; row++)
	{
		for (int col = 0; col < sizeCol; col++)
		{
			if (board[row][col]->IsRevealed())
			{
				board[row][col]->Hide();
			}
			else if (board[row][col]->IsFlagged())
			{
				board[row][col]->ClearFlag();
			}
		}
	}
}