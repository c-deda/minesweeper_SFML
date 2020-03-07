#ifndef GAME_H
#define GAME_H

#include <vector>
#include "tile.h"
#include "timer.h"
#include "Definitions.h"
#include "windowhandler.h"

class Game
{
public:
	enum State { NEW_GAME, ACTIVE, WIN, LOSE };
	enum Level { EASY, MEDIUM, HARD };

	Game();
	~Game();

	void DeleteBoard();
	void ResetGame(const Level level);
	void GenerateBoard(const int& clickedRow, const int& clickedCol);
	void GameLoop();
	void HandleInput();
	void Update();
	void Draw();
	void CountAdjacentMines(const int& row, const int& col, char** tempBoard, int& mineCount);
	void RevealBoard();
	void RevealTile(const int& row, const int& col);
	void FlagTile(const int& row, const int& col);
	bool CheckIfWon();
	void CountMinesRemaining();
	void CountAdjacentUnrevealedTiles(const int& row, const int& col, int& unrevealedSquares);
	void CountAdjacentFlags(const int& row, const int& col, int& adjacentFlags);
	void FlagAdjacentTiles(const int& row, const int& col);
	void RevealAdjacentTiles(const int& row, const int& col);
	void RehideBoard();

	void PrintBoard();

	bool IsSolvable(const int& clickedRow, const int& clickedCol);
private:
	WindowHandler windowHandler;
	std::vector<std::vector<Tile*>> board;
	Timer timer;
	State gameState;
	int sizeRow;
	int sizeCol;
	int mines;
	int minesRemaining;
};

#endif