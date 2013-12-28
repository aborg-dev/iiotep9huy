#pragma once

#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "model/Move.h"
#include "model/Tank.h"
#include "model/World.h"

class Strategy {
public:
	virtual ~Strategy();
	virtual void Move(model::Tank self, model::World world, model::Move& move) = 0;
	virtual model::TankType SelectTank(int tank_index, int team_size) = 0;
};

#endif
