#pragma once

#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "Unit.h"

namespace model {
	class Obstacle : public Unit {
	public:
		Obstacle(long long id, double width, double height, double x, double y);
	};
}

#endif
