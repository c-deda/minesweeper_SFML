#ifndef TILE_H
#define TILE_H

class Tile
{
public:
	Tile(const char& val);

	void Flag();
	void ClearFlag();
	void Reveal();
	void Hide();

	char GetVal() const { return this->val; }

	bool IsRevealed() const { return this->revealed; }
	bool IsFlagged() const { return this->flagged; }

	void SetVal(const char val);
private:
	char val;

	bool revealed;
	bool flagged;
};

#endif