#include "Shell.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace model;
using namespace std;

Shell::Shell(long long id, string player_name, double width, double height, double x, double y,
			 double speed_x, double speed_y, double angle, double angular_speed, ShellType type)
			 : Unit(id, width, height, x, y, speed_x, speed_y, angle, angular_speed), player_name_(player_name), type_(type) { }

string Shell::player_name() const {
	return player_name_; 
}

ShellType Shell::type() const {
	return type_;
}
