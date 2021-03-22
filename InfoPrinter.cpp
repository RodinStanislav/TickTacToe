#include "InfoPrinter.h"
#include "ConsoleManipulator.h"
#include "GameParams.h"
#include "Point2.h"

InfoPrinter& InfoPrinter::instance() {
	static InfoPrinter printer;
	return printer;
}

void InfoPrinter::init() {
	Point2 cursor = GameParams::instance().getMargin();

	ConsoleManipulator::instance().setCursorPosition(cursor);
	ConsoleManipulator::instance().putString("Let's play!");

	cursor.y += 2;
	ConsoleManipulator::instance().setCursorPosition(cursor);
	ConsoleManipulator::instance().putString("Press symbol (X / O / Esc(exit))");
}

GameSymbol InfoPrinter::chooseSymbol() {
	while (true) {
		if (GetAsyncKeyState(GameSymbol::X) & 0x8000) {
			return GameSymbol::X;
		}
		if (GetAsyncKeyState(GameSymbol::O) & 0x8000) {
			return GameSymbol::O;
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			return GameSymbol::Space;
		}
	}
}

void InfoPrinter::printWinner(GameSymbol symbol) {
	ConsoleManipulator::instance().clear();
	Point2 cursor = GameParams::instance().getMargin();

	std::string winner;
	if (symbol == GameSymbol::X) {
		winner = "X win!";
	}
	else if (symbol == GameSymbol::O) {
		winner = "O win!";
	}
	else {
		winner = "Draw...";
	}

	ConsoleManipulator::instance().setCursorPosition(cursor);
	ConsoleManipulator::instance().putString(winner);
	cursor.y += 2;

	ConsoleManipulator::instance().setCursorPosition(cursor);
	ConsoleManipulator::instance().putString("Go next? (X / O / Esc(exit))");
}