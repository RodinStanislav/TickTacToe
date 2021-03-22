#include "ConsoleManipulator.h"
#include "GameMap.h"
#include "InputHandler.h"
#include "GameParams.h"
#include "InfoPrinter.h"
#include "PlayerAI.h"
#include <cmath>

void Init() {
	// Начальная настройка цвета текста
	auto console = ConsoleManipulator::instance();
	console.setTextColor(ConsoleColor::White);

	ConsoleColor xColor = GameParams::instance().getColorX();
	ConsoleColor oColor = GameParams::instance().getColorO();

	// Настройка цвета игровых символов
	GameMap::instance().setSymbolColor(GameSymbol::X, xColor);
	GameMap::instance().setSymbolColor(GameSymbol::O, oColor);
	GameMap::instance().setSymbolColor(GameSymbol::Space, ConsoleColor::White);

	// Отображение вступительного текста
	InfoPrinter::instance().init();
}

void GameLoop() {
	while (true) {
		// Было предложено выбрать символ
		// Если возвращается SPACE, значит игрок захотел выйти

		std::size_t fieldSize = GameParams::instance().getFieldSize();
		std::size_t fieldSqr = static_cast<std::size_t>(std::pow(fieldSize, 2));

		GameSymbol symbol = InfoPrinter::instance().chooseSymbol();
		if (symbol == GameSymbol::Space) {
			return;
		}

		// Очищаем карту и выбирае символ первоходящего
		GameMap::instance().init(symbol);
		if (symbol == GameSymbol::X) {
			PlayerAI::instance().init(GameMap::instance().getGameMap(), GameSymbol::O);
		}
		else {
			PlayerAI::instance().init(GameMap::instance().getGameMap(), GameSymbol::X);
		}

		// Пока все поля не заполнены
		for (std::size_t i = 0; i < fieldSqr;) {

			// Проверка, что можно поставить символ
			bool result = false;
			while (result == false) {
				Point2 cell = InputHandler::instance().getClickedCell();
				result = GameMap::instance().setSymbol(cell.x, cell.y);
				i++;
			}

			// Обновление игровой карты
			GameMap::instance().update();

			GameSymbol winner = GameMap::instance().hasWinner();
			if (winner != GameSymbol::Space || i == fieldSqr) {
				InfoPrinter::instance().printWinner(winner);
				break;
			}

			Point2 cell = PlayerAI::instance().doTurn();
			// AI высчитывает ход так, что y - колонка, x - строка
			// Метод принимает первым аргументов колонку, поэтому необходимо свапнуть
			GameMap::instance().setSymbol(cell.y, cell.x);
			i++;

			// Обновление игровой карты
			GameMap::instance().update();

			// Проверка, что есть победитель
			winner = GameMap::instance().hasWinner();

			// Если победитель есть или же закончились поля,
			// Выходим из партии
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