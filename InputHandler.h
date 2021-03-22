#pragma once

#include "Point2.h"
#include <cstdio>

class InputHandler {
public:
	static InputHandler& instance();

	Point2 getClickedCell() const;

private:
	InputHandler();
	Point2 getSymbolOffset() const;
	Point2 getCursorPosition() const;
	Point2 waitInputCursorPosition() const;
};