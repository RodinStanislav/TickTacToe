#include "GameMap.h"
#include "ConsoleManipulator.h"
#include "Point2.h"
#include "GameParams.h"
#include <cmath>
#include <iostream>

static bool IsRadiusOfCircle(Point2 currentCoord, Point2 center, std::size_t radius, std::size_t offset = 0) {
	double currentRadius = std::pow(currentCoord.x - center.x, 2) +
		std::pow(currentCoord.y - center.y, 2);
	return (currentRadius >= (std::pow(radius, 2) - offset) && currentRadius <= (std::pow(radius, 2) + offset));
}

unsigned char GetGameSymbolIndex(GameSymbol symbol) {
	if (symbol == GameSymbol::X) {
		return 0;
	}
	else if (symbol == GameSymbol::O) {
		return 1;
	}
	else {
		return 2;
	}
}

GameMap& GameMap::instance() {
	static GameMap map;
	return map;
}

GameSymbol GameMap::getCurrentSymbol() {
	return m_currentSymbol;
}

GameSymbol** GameMap::getGameMap() {
	return m_map;
}

bool GameMap::setSymbol(std::size_t column, std::size_t row) {
	// В случае, если курсор не в игровом поле
	std::size_t fieldSize = GameParams::instance().getFieldSize();

	if (column > fieldSize - 1 || row > fieldSize - 1) {
		return false;
	}

	// Если ячейка уже занята
	if (m_map[column][row] != GameSymbol::Space) {
		return false;
	}

	// Ставим текущий символ и переключаемся
	m_map[column][row] = m_currentSymbol;
	if (m_currentSymbol == GameSymbol::X) {
		m_currentSymbol = GameSymbol::O;
	}
	else {
		m_currentSymbol = GameSymbol::X;
	}

	return true;
}

void GameMap::setSymbolColor(GameSymbol symbol, ConsoleColor color) {
	m_symbolColor[GetGameSymbolIndex(symbol)] = color;
}

GameSymbol GameMap::hasWinner() {
	// Проверка по вертикали
	std::size_t fieldSize = GameParams::instance().getFieldSize();

	for (std::size_t y = 0; y < fieldSize; y++) {
		if (m_map[y][0] == GameSymbol::Space) {
			continue;
		}
		else {
			GameSymbol symbol = m_map[y][0];
			bool result = true;
			for (std::size_t x = 1; x < fieldSize; x++) {
				if (symbol != m_map[y][x]) {
					result = false;
				}
			}

			if (result == true) {
				return symbol;
			}
		}
	}

	// Проверка по горизонтали
	for (std::size_t x = 0; x < fieldSize; x++) {
		if (m_map[0][x] == GameSymbol::Space) {
			continue;
		}
		else {
			GameSymbol symbol = m_map[0][x];
			bool result = true;
			for (std::size_t y = 1; y < fieldSize; y++) {
				if (symbol != m_map[y][x]) {
					result = false;
					break;
				}
			}

			if (result == true) {
				return symbol;
			}
		}
	}

	// Главная диагональ
	if (m_map[0][0] != GameSymbol::Space) {
		GameSymbol symbol = m_map[0][0];
		bool result = true;
		for (std::size_t i = 1; i < fieldSize; i++) {
			if (symbol != m_map[i][i]) {
				result = false;
				break;
			}
		}

		if (result == true) {
			return symbol;
		}
	}

	// Побочная диагональ
	if (m_map[fieldSize - 1][0] != GameSymbol::Space) {
		GameSymbol symbol = m_map[fieldSize - 1][0];
		bool result = true;
		for (std::size_t i = 1; i < fieldSize; i++) {
			if (symbol != m_map[(int)fieldSize - 1 - i][i]) {
				result = false;
				break;
			}
		}

		if (result == true) {
			return symbol;
		}
	}

	return GameSymbol::Space;
}

void GameMap::update() {
	// Перерисовка консоли
	// Опитимизация: перерисовывать только ту часть буфера, 
	// где заменяется символы
	std::size_t fieldSize = GameParams::instance().getFieldSize();
	Point2 margin = GameParams::instance().getMargin();
	ConsoleManipulator::instance().clear();

	for (std::size_t y = 0; y < fieldSize; y++) {
		for (std::size_t x = 0; x < fieldSize; x++) {
			// Печатаем символ
			setRow(x);
			setColumn(y);
			printSymbol(m_map[y][x]);

			// Определяем, где находится каретка
			Point2 cursorPos = ConsoleManipulator::instance().getCursorPosition();
			cursorPos.x = margin.x + (x + 1) * getRealSize();
			if (x > 0) {
				cursorPos.x += x * 3;
			}
			cursorPos.y = margin.y + y * getRealSize();
			if (y > 0) {
				cursorPos.y += y * 3;
			}
			ConsoleManipulator::instance().setCursorPosition(cursorPos);

			
			if (x != fieldSize - 1) {
				printVerticalDelimeter();
			}
		}

		Point2 cursorPos = ConsoleManipulator::instance().getCursorPosition();
		cursorPos.x = margin.x;
		cursorPos.y = margin.y + (y + 1) * getRealSize();
		if (y > 0) {
			cursorPos.y += y * 3;
		}
		ConsoleManipulator::instance().setCursorPosition(cursorPos);
		if (y != fieldSize - 1) {
			printHorizontalDelimeter();
		}
	}
}

void GameMap::init(GameSymbol symbol) {
	std::size_t fieldSize = GameParams::instance().getFieldSize();

	m_currentSymbol = symbol;

	for (std::size_t y = 0; y < fieldSize; y++) {
		for (std::size_t x = 0; x < fieldSize; x++) {
			m_map[y][x] = GameSymbol::Space;
		}
	}

	update();
}

std::size_t GameMap::getRealSize() const {
	// На самом деле size - радиус символа. 
	// Для правильного отображения необходимо нечетное число
	std::size_t symbolSize = GameParams::instance().getSymbolSize();

	return (symbolSize * 2 - 1);
}
// Передвигаем каретку на позицию n-ной строки
void GameMap::setRow(std::size_t row) {

	Point2 margin = GameParams::instance().getMargin();
	std::size_t fieldSize = GameParams::instance().getFieldSize();

	if (row > fieldSize - 1) {
		std::cout << "ERROR in GameMap::setRow: " + std::to_string(row);
	}
	Point2 cursorPos = ConsoleManipulator::instance().getCursorPosition();
	cursorPos.y = margin.y + row * (getRealSize() + 3);
	ConsoleManipulator::instance().setCursorPosition(cursorPos);
}

// Передвигаем каретку на позицию n-ной колонки
void GameMap::setColumn(std::size_t column) {
	Point2 margin = GameParams::instance().getMargin();
	std::size_t fieldSize = GameParams::instance().getFieldSize();

	if (column > fieldSize - 1) {
		std::cout << "ERROR in GameMap::setColumn: " + std::to_string(column);
	}
	Point2 cursorPos = ConsoleManipulator::instance().getCursorPosition();
	cursorPos.x = margin.x + column * (getRealSize() + 3);
	ConsoleManipulator::instance().setCursorPosition(cursorPos);
}

GameMap::GameMap() {

	std::size_t fieldSize = GameParams::instance().getFieldSize();

	m_map = new GameSymbol*[fieldSize];
	for (std::size_t i = 0; i < fieldSize; i++) {
		m_map[i] = new GameSymbol[fieldSize];
	}

	for (std::size_t y = 0; y < fieldSize; y++) {
		for (std::size_t x = 0; x < fieldSize; x++) {
			m_map[y][x] = GameSymbol::Space;
		}
	}

	for (std::size_t i = 0; i < 3; i++) {
		m_symbolColor[i] = ConsoleColor::White;
	}
	m_currentSymbol = GameSymbol::X;
}

GameMap::~GameMap() {

	std::size_t fieldSize = GameParams::instance().getFieldSize();

	if (m_map != nullptr) {
		for (std::size_t y = 0; y < fieldSize; y++) {
			if (m_map[y] != nullptr) {
				delete[] m_map[y];
				m_map[y] = nullptr;
			}
		}
		delete[] m_map;
		m_map = nullptr;
	}
}

void GameMap::printSpace() const {
	Point2 currentCursorPos = ConsoleManipulator::instance().getCursorPosition();

	std::size_t realSize = getRealSize();
	std::string string;
	for (std::size_t y = 0; y < realSize; y++) {
		ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
		for (std::size_t x = 0; x < realSize; x++) {
			string = string + "1";
		}
		ConsoleManipulator::instance().putString(string);
		string.clear();
		currentCursorPos.y += 1;
	}
	currentCursorPos.y -= 1;
	currentCursorPos.x += realSize;
	ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
}

void GameMap::printX() const {
	Point2 currentCursorPos = ConsoleManipulator::instance().getCursorPosition();

	std::size_t realSize = getRealSize();
	std::size_t offset = 0;
	std::string string;

	for (std::size_t y = 0; y < realSize; y++) {
		for (std::size_t x = 0; x < realSize; x++) {
			ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
			if (x == (realSize - offset - 1) || x == offset) {
				string = string + static_cast<char>(GameSymbol::X);
			}
			else {
				string = string + " ";
			}
		}
		ConsoleManipulator::instance().putString(string);
		string.clear();
		currentCursorPos.y += 1;
		offset++;
	}

	currentCursorPos.y -= 1;
	currentCursorPos.x += realSize;
	ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
}

void GameMap::printO() const {
	Point2 currentCursorPos = ConsoleManipulator::instance().getCursorPosition();

	std::size_t realSize = getRealSize();
	std::size_t radius = realSize / 2;

	Point2 currentCoord;
	Point2 center{ static_cast<int>(radius), static_cast<int>(radius) };

	std::string string;

	for (std::size_t y = 0; y < realSize; y++) {
		for (std::size_t x = 0; x < realSize; x++) {
			ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
			currentCoord.x = x;
			currentCoord.y = y;
			if (IsRadiusOfCircle(currentCoord, center, radius, radius)) {
				string = string + static_cast<char>(GameSymbol::O);
			}
			else {
				string = string + " ";
			}
		}
		ConsoleManipulator::instance().putString(string);
		string.clear();
		currentCursorPos.y += 1;
	}

	currentCursorPos.y -= 1;
	currentCursorPos.x += realSize;
	ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
}

void GameMap::printVerticalDelimeter() const {
	Point2 currentCursorPos = ConsoleManipulator::instance().getCursorPosition();

	std::size_t realSize = getRealSize();
	for (std::size_t y = 0; y < realSize; y++) {
		ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
		ConsoleManipulator::instance().putString(" | ");
		currentCursorPos.y += 1;
	}

	currentCursorPos.x += 3;
	currentCursorPos.y -= 1;
	ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
}

void GameMap::printHorizontalDelimeter() const {
	Point2 currentCursorPos = ConsoleManipulator::instance().getCursorPosition();
	std::size_t fieldSize = GameParams::instance().getFieldSize();

	std::size_t realSize = getRealSize();
	std::size_t fieldWidth = realSize * fieldSize + 3 * (fieldSize - 1);
	for (std::size_t x = 0; x < fieldWidth; x++) {
		ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
		ConsoleManipulator::instance().putString(" ");
		currentCursorPos.x += 1;
	}
	currentCursorPos.x -= fieldWidth;
	currentCursorPos.y += 1;
	for (std::size_t x = 0; x < fieldWidth; x++) {
		ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
		ConsoleManipulator::instance().putString("-");
		currentCursorPos.x += 1;
	}
	currentCursorPos.x -= fieldWidth;
	currentCursorPos.y += 1;
	for (std::size_t x = 0; x < fieldWidth; x++) {
		ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
		ConsoleManipulator::instance().putString(" ");
		currentCursorPos.x += 1;
	}
	
	ConsoleManipulator::instance().setCursorPosition(currentCursorPos);
}

void GameMap::printSymbol(GameSymbol symbol) const {
	ConsoleColor oldColor = ConsoleManipulator::instance().getTextColor();
	ConsoleManipulator::instance().setTextColor(m_symbolColor[GetGameSymbolIndex(symbol)]);
	switch (symbol)
	{
	case Space:
		printSpace();
		break;
		return;
	case X:
		printX();
		break;
	case O:
		printO();
		break;
	}
	ConsoleManipulator::instance().setTextColor(oldColor);
	return;
}