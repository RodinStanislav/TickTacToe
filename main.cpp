#include "ConsoleManipulator.h"
#include "GameMap.h"
#include "InputHandler.h"
#include "GameParams.h"
#include "InfoPrinter.h"
#include "PlayerAI.h"
#include <cmath>

void Init() {
	// ��������� ��������� ����� ������
	auto console = ConsoleManipulator::instance();
	console.setTextColor(ConsoleColor::White);

	ConsoleColor xColor = GameParams::instance().getColorX();
	ConsoleColor oColor = GameParams::instance().getColorO();

	// ��������� ����� ������� ��������
	GameMap::instance().setSymbolColor(GameSymbol::X, xColor);
	GameMap::instance().setSymbolColor(GameSymbol::O, oColor);
	GameMap::instance().setSymbolColor(GameSymbol::Space, ConsoleColor::White);

	// ����������� �������������� ������
	InfoPrinter::instance().init();
}

void GameLoop() {
	while (true) {
		// ���� ���������� ������� ������
		// ���� ������������ SPACE, ������ ����� ������� �����

		std::size_t fieldSize = GameParams::instance().getFieldSize();
		std::size_t fieldSqr = static_cast<std::size_t>(std::pow(fieldSize, 2));

		GameSymbol symbol = InfoPrinter::instance().chooseSymbol();
		if (symbol == GameSymbol::Space) {
			return;
		}

		// ������� ����� � ������� ������ �������������
		GameMap::instance().init(symbol);
		if (symbol == GameSymbol::X) {
			PlayerAI::instance().init(GameMap::instance().getGameMap(), GameSymbol::O);
		}
		else {
			PlayerAI::instance().init(GameMap::instance().getGameMap(), GameSymbol::X);
		}

		// ���� ��� ���� �� ���������
		for (std::size_t i = 0; i < fieldSqr;) {

			// ��������, ��� ����� ��������� ������
			bool result = false;
			while (result == false) {
				Point2 cell = InputHandler::instance().getClickedCell();
				result = GameMap::instance().setSymbol(cell.x, cell.y);
				i++;
			}

			// ���������� ������� �����
			GameMap::instance().update();

			GameSymbol winner = GameMap::instance().hasWinner();
			if (winner != GameSymbol::Space || i == fieldSqr) {
				InfoPrinter::instance().printWinner(winner);
				break;
			}

			Point2 cell = PlayerAI::instance().doTurn();
			// AI ����������� ��� ���, ��� y - �������, x - ������
			// ����� ��������� ������ ���������� �������, ������� ���������� ��������
			GameMap::instance().setSymbol(cell.y, cell.x);
			i++;

			// ���������� ������� �����
			GameMap::instance().update();

			// ��������, ��� ���� ����������
			winner = GameMap::instance().hasWinner();

			// ���� ���������� ���� ��� �� ����������� ����,
			// ������� �� ������
			if (winner != GameSymbol::Space) {
				InfoPrinter::instance().printWinner(winner);
				break;
			}
		}
	}
}

int main() {	
	Init();
	GameLoop();
	return EXIT_SUCCESS;
}