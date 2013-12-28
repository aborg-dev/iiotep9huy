#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"

class MyStrategy : public Strategy {
public:
	void Move(model::Tank self, model::World world, model::Move& move);
	model::TankType SelectTank(int tank_index, int team_size);
};

#endif
