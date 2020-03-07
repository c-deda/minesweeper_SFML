#include "windowhandler.h"
#include <iomanip>
#include <sstream>
#include <cstdlib>

WindowHandler::WindowHandler()
{
	this->window.create(sf::VideoMode(EASY_WIN_WIDTH, EASY_WIN_HEIGHT), TITLE, sf::Style::Close | sf::Style::Titlebar);
	this->window.setFramerateLimit(FRAMERATE);
	this->view.setCenter(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	this->view.setSize(sf::Vector2f(EASY_WIN_WIDTH, EASY_WIN_HEIGHT));

	Init();
}

void WindowHandler::Init()
{
	tiles.setTexture(LoadAndGetTextures(TILES_PATH));
	buttons.setTexture(LoadAndGetTextures(BUTTONS_PATH));
	buttons.setOrigin((BUTTON_WIDTH * 3) / 2, 0);
	timerText.setFont(LoadAndGetFonts(FONT_PATH));
	mineCounterText.setFont(LoadAndGetFonts(FONT_PATH));
	winLoseText.setFont(LoadAndGetFonts(FONT_PATH));

	timerText.setCharacterSize(TILE_SIZE * 1.5);
	mineCounterText.setCharacterSize(TILE_SIZE * 1.5);
	winLoseText.setCharacterSize(TILE_SIZE);

	timerText.setFillColor(sf::Color::Black);
	mineCounterText.setFillColor(sf::Color::Black);
	winLoseText.setFillColor(sf::Color::Black);

	timerText.setPosition((window.getSize().x / 2) + (TILE_SIZE * 2.5), TILE_SIZE / 2);
	mineCounterText.setPosition((window.getSize().x / 2) - (TILE_SIZE * 4), TILE_SIZE / 2);
	winLoseText.setPosition((window.getSize().x / 2) - (TILE_SIZE * 1.25), TILE_SIZE);
}

void WindowHandler::DrawTimer(int time)
{
	std::stringstream ss;
	std::string s;

	ss << std::setfill('0') << std::setw(3) << time;
	ss >> s;

	timerText.setString(s);
	window.draw(timerText);
}

void WindowHandler::DrawMineCounter(int& mineCount)
{
	std::stringstream ss;
	std::string s;

	if (mineCount < 0)
	{
		ss << "-" << std::setfill('0') << std::setw(3) << abs(mineCount);
		ss >> s;
	}
	else
	{
		ss << std::setfill('0') << std::setw(3) << mineCount;
		ss >> s;
	}

	mineCounterText.setString(s);
	window.draw(mineCounterText);
}

void WindowHandler::DrawTiles(const int& x, const int& y, const char& value)
{
	int textureValue;

	switch (value)
	{
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':
		textureValue = value - '0';
		break;
	case '*':
		textureValue = 9;
		break;
	case 'U':
		textureValue = 10;
		break;
	case 'F':
		textureValue = 11;
		break;
	case '!':
		textureValue = 12;
		break;
	}

	tiles.setTextureRect(sf::IntRect(textureValue * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
	tiles.setPosition((x + 1)*TILE_SIZE, (y + 3)*TILE_SIZE);
	window.draw(tiles);
}

void WindowHandler::DrawLevelButtons()
{
	buttons.setPosition(window.getSize().x / 2, window.getSize().y - (TILE_SIZE*1.25));
	window.draw(buttons);
}

void WindowHandler::DrawWinLoseText(const std::string& string)
{
	winLoseText.setString(string);
	window.draw(winLoseText);
}

sf::Texture& WindowHandler::LoadAndGetTextures(const std::string& filename)
{
	sf::Texture texture;

	if (texture.loadFromFile(filename))
	{
		this->textures[filename] = texture;
	}

	return this->textures.at(filename);
}

sf::Font& WindowHandler::LoadAndGetFonts(const std::string& filename)
{
	sf::Font font;

	if (font.loadFromFile(filename))
	{
		this->fonts[filename] = font;
	}

	return this->fonts.at(filename);
}

char WindowHandler::LevelButtonClicked(const int& x, const int& y)
{
	int buttonsLeft = buttons.getGlobalBounds().left;
	int buttonsRight = buttons.getGlobalBounds().width + buttonsLeft;
	int buttonsTop = buttons.getGlobalBounds().top;
	int buttonsBottom = buttonsTop + BUTTON_HEIGHT;

	// EASY BUTTON
	if ((x >= buttonsLeft && x <= buttonsRight - (BUTTON_WIDTH * 2)) &&
		(y >= buttonsTop && y <= buttonsBottom))
	{
		return 'e';
	}
	// MEDIUM BUTTON
	else if ((x >= buttonsLeft + BUTTON_WIDTH && x <= buttonsRight - (BUTTON_WIDTH * 1)) &&
		(y >= buttonsTop && y <= buttonsBottom))
	{
		return 'm';
	}
	// HARD BUTTON
	else if ((x >= buttonsLeft + (BUTTON_WIDTH * 2) && x <= buttonsRight) &&
		(y >= buttonsTop && y <= buttonsBottom))
	{
		return 'h';
	}
	
	return ' ';
}

void WindowHandler::Resize(const int& width, const int& height)
{
	window.setSize(sf::Vector2u(width, height));
	view.setCenter(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	view.setSize(width, height);

	Init();
}