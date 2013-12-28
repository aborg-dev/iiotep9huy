#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>

namespace model {
	class Player {
	private:
		std::string name_;
		int score_;
		bool strategy_crashed_;
	public:
		Player(std::string name, int score, bool strategy_crashed);

		std::string name() const;
		int score() const;
		bool strategy_crashed() const;
	};
}

#endif
