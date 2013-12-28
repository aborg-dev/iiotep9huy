#pragma once

#ifndef _SHELL_H_
#define _SHELL_H_

#define _USE_MATH_DEFINES
#include <string>

#include "Unit.h"

namespace model {
	enum ShellType {
		UNKNOWN_SHELL = -1,
		REGULAR = 0,
		PREMIUM = 1
	};

	class Shell : public Unit {
	private:
		std::string player_name_;
		ShellType type_;
	public:
		Shell(long long id, std::string player_name, double width, double height, double x, double y,
			double speed_x, double speed_y, double angle, double angular_speed, ShellType type);

		std::string player_name() const;
		ShellType type() const;
	};
}

#endif
