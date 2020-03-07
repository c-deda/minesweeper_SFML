#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include "SFML/Graphics.hpp"
#include <map>
#include <string>
#include "Definitions.h"

class WindowHandler
{
public:
	sf::RenderWindow window;
	sf::View view;

	WindowHandler();

	void Init();
	void DrawTimer(int time);
	void DrawMineCounter(int& mineCount);
	void DrawTiles(const int& x, const int& y, const char& value);
	void DrawLevelButtons();
	void DrawWinLoseText(const std::string& s);
	void Resize(const int& width, const int& height);

	char LevelButtonClicked(const int& x, const int& y);

	sf::Texture& LoadAndGetTextures(const std::string& fileName);
	sf::Font& LoadAndGetFonts(const std::string& filename);
private:
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	sf::Sprite tiles;
	sf::Sprite buttons;
	sf::Text timerText;
	sf::Text mineCounterText;
	sf::Text winLoseText;
};

#endif