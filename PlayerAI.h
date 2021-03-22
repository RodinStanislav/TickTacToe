#pragma once

#include "GameMap.h"
#include "GameParams.h"

class PlayerAI {
public:
	static PlayerAI& instance();

	void init(GameSymbol** map, GameSymbol symbol);

	Point2 doTurn() const;

private:

	PlayerAI() = default;

	Point2 tryWinNow(GameSymbol symbol) const;
	Point2 tryNotLose() const;

	GameSymbol** m_map;
	GameSymbol m_aiSymbol;
};