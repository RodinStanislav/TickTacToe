#pragma once

#include <cstdio>
#include "ConsoleColor.h"

enum GameSymbol {
	Space = ' ',
	X = 'X',
	O = 'O'
};

unsigned char GetGameSymbolIndex(GameSymbol symbol);

class GameMap {
public:
	static GameMap& instance();
	void init(GameSymbol symbol);

	bool setSymbol(std::size_t column, std::size_t row);
	void setSymbolColor(GameSymbol symbol, ConsoleColor color);
	GameSymbol hasWinner();
	GameSymbol** getGameMap();

	GameSymbol getCurrentSymbol();

	void update();
	~GameMap();

private:
	void setRow(std::size_t row);
	void setColumn(std::size_t column);

	void printVerticalDelimeter() const;
	void printHorizontalDelimeter() const;

	void printSpace() const;
	void printX() const;
	void printO() const;

	void printSymbol(GameSymbol symbol) const;
	std::size_t getRealSize() const;

	GameMap();

	GameSymbol** m_map;
	GameSymbol m_currentSymbol;
	ConsoleColor m_symbolColor[3];
};