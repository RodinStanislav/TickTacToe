#pragma once

#include <cstdio>
#include <string>
#include <ctime>
#include "Point2.h"
#include "ConsoleColor.h"

class GameParams {
public:
	static GameParams& instance();
	void init(const std::string& configFilename = "config.ini");

	Point2 getResolution() const;
	Point2 getMargin() const;
	ConsoleColor getColorX() const;
	ConsoleColor getColorO() const;
	std::size_t getFieldSize() const;
	std::size_t getSymbolSize() const;

private:

	GameParams();

	Point2 m_resolution;
	Point2 m_margin;
	std::size_t m_symbolSize;
	std::size_t m_fieldSize;
	ConsoleColor m_xColor;
	ConsoleColor m_oColor;
};