#include "tile.h"

Tile::Tile(const char& val)
{
	this->val = val;
	this->flagged = false;
	this->revealed = false;
}

void Tile::Flag()
{
	this->flagged = true;
}

void Tile::ClearFlag()
{
	this->flagged = false;
}

void Tile::Reveal()
{
	this->revealed = true;
}

void Tile::Hide()
{
	this->revealed = false;
}

void Tile::SetVal(const char val)
{
	switch (val)
	{
	case '0':case '1': case'2': case '3': case '4': case '5': case '6': case '7': case '8': case '*': case '!':
		this->val = val;
		break;
	}
}