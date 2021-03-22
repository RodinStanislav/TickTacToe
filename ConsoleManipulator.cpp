#include "ConsoleManipulator.h"
#include "GameParams.h"
#include <iostream>

ConsoleManipulator& ConsoleManipulator::instance() {
	static ConsoleManipulator manipulator;
	return manipulator;
}

void ConsoleManipulator::setTextColor(ConsoleColor color) {
	m_textColor = color;
	SetConsoleTextAttribute(m_consoleOutputHandle, (WORD)((m_backgroundColor << 4) | m_textColor));
}

void ConsoleManipulator::setBackgroundColor(ConsoleColor color) {
	m_backgroundColor = color;
	SetConsoleTextAttribute(m_consoleOutputHandle, (WORD)((m_backgroundColor << 4) | m_textColor));
}

void ConsoleManipulator::putString(const std::string& string) {
	std::cout << string;
}

ConsoleColor ConsoleManipulator::getTextColor() const {
	return m_textColor;
}

ConsoleColor ConsoleManipulator::getBackgroundColor() const {
	return m_backgroundColor;
}

void ConsoleManipulator::setCursorPosition(Point2 cursorPos) {
	m_cursorPos = cursorPos;
	SetConsoleCursorPosition(m_consoleOutputHandle, getCoord());
}

Point2 ConsoleManipulator::getCursorPosition() const {
	return m_cursorPos;
}

ConsoleManipulator::~ConsoleManipulator() {
	GetConsoleMode(m_consoleInputHandle, &m_oldMode);
}

COORD ConsoleManipulator::getCoord() {
	COORD coord;
	coord.X = m_cursorPos.x;
	coord.Y = m_cursorPos.y;
	return coord;
}

void ConsoleManipulator::clear() const {
	system("cls");
}

RECT ConsoleManipulator::getConsoleRect() const {
	RECT rect;
	HWND hWnd = GetConsoleWindow();
	GetWindowRect(hWnd, &rect);
	return rect;
}

ConsoleManipulator::ConsoleManipulator() {
	// Получаем дескрипторы консоли
	// Std_input для обработки мыши и клавиатуры
	m_consoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);

	// Std_output для вывода текста
	m_consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTitleA("TickTacToe");

	Point2 resolution = GameParams::instance().getResolution();

	// Центрируем окно консоли
	int x = (GetSystemMetrics(SM_CXSCREEN) - resolution.x) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - resolution.y) / 2;

	HWND consoleHWnd = GetConsoleWindow();
	SetWindowPos(consoleHWnd, HWND_TOP, x, y,
				 resolution.x, resolution.y, NULL);

	// Запоминаем старый стиль
	// После закрытия игры возвращаем все на место (в деструкторе)
	DWORD mode;
	GetConsoleMode(m_consoleInputHandle, &m_oldMode);
	mode = m_oldMode;
	mode |= ENABLE_WINDOW_INPUT;		// Включаем взаимодействие с мышью
	mode &= ~ENABLE_QUICK_EDIT_MODE;	// Отключаем копирование мышью
	mode |= ENABLE_EXTENDED_FLAGS;		// Включаем расширенный флаг (для работы предыдущего
	SetConsoleMode(m_consoleInputHandle, mode);

	// Скрываем курсор
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(m_consoleOutputHandle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(m_consoleOutputHandle, &structCursorInfo);

	m_backgroundColor = ConsoleColor::Black;
	m_textColor = ConsoleColor::White;
	m_cursorPos = Point2(0, 0);
}


	