#include "PlayerAI.h"

PlayerAI& PlayerAI::instance() {
	static PlayerAI ai;
	return ai;
}

void PlayerAI::init(GameSymbol** map, GameSymbol symbol) {
	m_map = map;
	m_aiSymbol = symbol;
}

Point2 PlayerAI::doTurn() const {
	std::size_t fieldSize = GameParams::instance().getFieldSize();
	Point2 cell = tryWinNow(m_aiSymbol);
	if (cell.x == -1 && cell.y == -1) {
		cell = tryNotLose();
	}

	if (cell.x == -1 && cell.y == -1) {
		while (true) {
			int x = rand() % fieldSize;
			int y = rand() % fieldSize;
			if (m_map[y][x] == GameSymbol::Space) {
				return Point2(x, y);
			}
		}
	}

	return cell;
}

Point2 PlayerAI::tryWinNow(GameSymbol symbol) const {
	std::size_t fieldSize = GameParams::instance().getFieldSize();

	for (std::size_t y = 0; y < fieldSize; y++) {
		std::size_t countSymbolInRow = 0;
		for (std::size_t x = 0; x < fieldSize; x++) {
			if (m_map[y][x] == symbol) {
				countSymbolInRow++;
				continue;
			}
			if (m_map[y][x] == GameSymbol::Space) {
				continue;
			}

			break;
		}
		if (countSymbolInRow == fieldSize - 1) {
			for (std::size_t x = 0; x < fieldSize; x++) {
				if (m_map[y][x] == GameSymbol::Space) {
					return Point2(x, y);
				}
			}
		}
	}

	// Проверка по горизонтали
	for (std::size_t x = 0; x < fieldSize; x++) {
		std::size_t countSymbolInColumn = 0;
		for (std::size_t y = 0; y < fieldSize; y++) {
			if (m_map[y][x] == symbol) {
				countSymbolInColumn++;
				continue;
			}
			if (m_map[y][x] == GameSymbol::Space) {
				continue;
			}

			break;
		}
		if (countSymbolInColumn == fieldSize - 1) {
			for (std::size_t y = 0; y < fieldSize; y++) {
				if (m_map[y][x] == GameSymbol::Space) {
					return Point2(x, y);
				}
			}
		}
	}

	std::size_t countSymbolInDiagonal = 0;
	for (std::size_t i = 0; i < fieldSize; i++) {
		if (m_map[i][i] == symbol) {
			countSymbolInDiagonal++;
			continue;
		}
		if (m_map[i][i] == GameSymbol::Space) {
			continue;
		}

		break;
	}

	if (countSymbolInDiagonal == fieldSize - 1) {
		for (std::size_t i = 0; i < fieldSize; i++) {
			if (m_map[i][i] == GameSymbol::Space) {
				return Point2(i, i);
			}
		}
	}

	countSymbolInDiagonal = 0;

	// Побочная диагональ
	for (std::size_t i = 0; i < fieldSize; i++) {
		if (m_map[i][fieldSize - i - 1] == symbol) {
			countSymbolInDiagonal++;
			continue;
		}
		if (m_map[i][fieldSize - i - 1] == GameSymbol::Space) {
			continue;
		}

		break;
	}

	if (countSymbolInDiagonal == fieldSize - 1) {
		for (std::size_t i = 0; i < fieldSize; i++) {
			if (m_map[i][fieldSize - i - 1] == GameSymbol::Space) {
				return Point2(fieldSize - i - 1, i);
			}
		}
	}

	return Point2(-1, -1);
}

Point2 PlayerAI::tryNotLose() const {
	if (m_aiSymbol == GameSymbol::X) {
		return tryWinNow(GameSymbol::O);
	}
	return tryWinNow(GameSymbol::X);
}