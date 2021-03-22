#include "InputHandler.h"
#include "ConsoleManipulator.h"
#include "Point2.h"
#include "GameParams.h"
#include <Windows.h>

InputHandler& InputHandler::instance() {
	static InputHandler handler;
	return handler;
}

Point2 InputHandler::getClickedCell() const {
	Point2 cursorPos = waitInputCursorPosition();
	Point2 offset = getSymbolOffset();

	Point2 margin = GameParams::instance().getMargin();
	// Определяем, на какой символ мы попали (с учетом внешнего отступа)
	Point2 cell;
	cell.x = cursorPos.x / offset.x - margin.x;
	cell.y = cursorPos.y / offset.y - margin.y;

	std::size_t symbolSize = GameParams::instance().getSymbolSize();
	// Далее определяем, к какой ячейке принадлежит символ
	std::size_t realSize = symbolSize * 2 - 1;
	cell.x = cell.x / (realSize + 3);
	cell.y = cell.y / (realSize + 3);
	return cell;
}

InputHandler::InputHandler() {}

Point2 InputHandler::getSymbolOffset() const {
	// размер символа (примерный)
	return Point2(9, 13);
}

Point2 InputHandler::getCursorPosition() const {
	POINT cursor;
	RECT rect = ConsoleManipulator::instance().getConsoleRect();
	GetCursorPos(&cursor);

	// 7:30 - непонятный стандартный оффсет
	// Конвертируем координаты мыши из пространства монитора в 
	// пространство консоли
	return Point2(cursor.x - rect.left - 7, cursor.y - rect.top - 30);
}

Point2 InputHandler::waitInputCursorPosition() const {
	while (true) {
		// Если нажата левая кнопка мыши
		if (GetKeyState(VK_LBUTTON) & 0x8000) {
			return getCursorPosition();
		}
	}
}