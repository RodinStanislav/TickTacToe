#pragma once

#include "GameMap.h"

class InfoPrinter {
public:
	static InfoPrinter& instance();

	void init();
	GameSymbol chooseSymbol();

	void printWinner(GameSymbol symbol);

private:
	InfoPrinter() = default;
};