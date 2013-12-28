#pragma once

#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>

#include "Bonus.h"
#include "Obstacle.h"
#include "Player.h"
#include "Shell.h"
#include "Tank.h"

namespace model {
	class World {
	private:
		int tick_;
		double width_;
		double height_;
		std::vector<Player> players_;
		std::vector<Obstacle> obstacles_;
		std::vector<Tank> tanks_;
		std::vector<Shell> shells_;
		std::vector<Bonus> bonuses_;
	public:
		World(int tick, double width, double height, const std::vector<Player>& players, const std::vector<Obstacle>& obstacles,
			const std::vector<Tank>& tanks, const std::vector<Shell>& shells, const std::vector<Bonus>& bonuses);

		int tick() const;
		double width() const;
		double height() const;
		std::vector<Player> players() const;
		std::vector<Obstacle> obstacles() const;
		std::vector<Tank> tanks() const;
		std::vector<Shell> shells() const;
		std::vector<Bonus> bonuses() const;
	};
}

#endif
