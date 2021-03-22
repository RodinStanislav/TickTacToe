#include "GameParams.h"

GameParams& GameParams::instance() {
	static GameParams params;
	return params;
}

void GameParams::init(const std::string& configFilename) {
	// Есть возможность загружать конфиг из файла
	// Если конфига нет - создать дефолтный
	// Если есть - загрузить
	// В случае ошибки парсинга аргумента - 
	// отмена операции, стандартные настройки, вывод ошибки

	// Сейчас не реализовано, так как и так перегружено для тестовой работы
}

Point2 GameParams::getResolution() const {
	return m_resolution;
}

Point2 GameParams::getMargin() const {
	return m_margin;
}

ConsoleColor GameParams::getColorX() const {
	return m_xColor;
}

ConsoleColor GameParams::getColorO() const {
	return m_oColor;
}

std::size_t GameParams::getFieldSize() const {
	return m_fieldSize;
}

std::size_t GameParams::getSymbolSize() const {
	return m_symbolSize;
}

GameParams::GameParams() {
	srand(time(0));

	// m_fieldSize >= 3
	m_fieldSize = 3;
	m_margin = Point2((int)(3.5f * m_fieldSize), m_fieldSize);
	m_resolution = Point2(150 * m_fieldSize, 170 * m_fieldSize);
	m_symbolSize = 4;

	m_xColor = ConsoleColor::LightRed;
	m_oColor = ConsoleColor::LightBlue;
}
	