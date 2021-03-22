#pragma once

#include <Windows.h>
#include <string>
#include "Point2.h"
#include "ConsoleColor.h"
#include "GameParams.h"

class ConsoleManipulator {
public:
	static ConsoleManipulator& instance();

	void setTextColor(ConsoleColor color);
	void setBackgroundColor(ConsoleColor color);
	ConsoleColor getTextColor() const;
	ConsoleColor getBackgroundColor() const;

	void putString(const std::string& string);
	
	void setCursorPosition(Point2 cursorPos);
	Point2 getCursorPosition() const;
	RECT getConsoleRect() const;
	void clear() const;

	~ConsoleManipulator();

private:

	COORD getCoord();

	ConsoleManipulator();

	HANDLE m_consoleOutputHandle;
	HANDLE m_consoleInputHandle;
	ConsoleColor m_backgroundColor;
	ConsoleColor m_textColor;
	Point2 m_cursorPos;

	DWORD m_oldMode;
};