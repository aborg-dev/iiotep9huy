#include "Tank.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace model;
using namespace std;

Tank::Tank(long long id, string player_name, int teammate_index, double x, double y,
		   double speed_x, double speed_y, double angle, double angular_speed,
		   double turret_relative_angle, int crew_health, int hull_durability,
		   int reloading_time, int remaining_reloading_time, int premium_shell_count, bool teammate, TankType type)
		   : Unit(id, GetWidth(type), GetHeight(type), x, y, speed_x, speed_y, angle, angular_speed),
		   player_name_(player_name), teammate_index_(teammate_index),
		   turret_relative_angle_(turret_relative_angle), crew_health_(crew_health), hull_durability_(hull_durability),
		   reloading_time_(reloading_time), remaining_reloading_time_(remaining_reloading_time),
		   premium_shell_count_(premium_shell_count), teammate_(teammate), type_(type) { }

double Tank::GetWidth(const TankType type) const {
	switch (type) {
		case MEDIUM:
			return 90.0;
		case HEAVY:
			return 105.0;
		case TANK_DESTROYER:
			return 112.5;
		default:
			throw;
	}
}

double Tank::GetHeight(const TankType type) const {
	switch (type) {
		case MEDIUM:
			return 60.0;
		case HEAVY:
			return 75.0;
		case TANK_DESTROYER:
			return 67.5;
		default:
			throw;
	}
}

string Tank::player_name() const {
	return player_name_; 
}

int Tank::teammate_index() const {
	return teammate_index_;
}

double Tank::turret_relative_angle() const {
	return turret_relative_angle_;
}

int Tank::crew_health() const {
	return crew_health_;
}

int Tank::hull_durability() const {
	return hull_durability_;
}

int Tank::reloading_time() const {
	return reloading_time_;
}

int Tank::remaining_reloading_time() const {
	return remaining_reloading_time_;
}

int Tank::premium_shell_count() const {
	return premium_shell_count_;
}

bool Tank::teammate() const {
	return teammate_;
}

TankType Tank::type() const {
	return type_;
}

double Tank::virtual_gun_length() const {
	switch (type_) {
		case MEDIUM:
			return 67.5;
		case HEAVY:
			return 82.5;
		case TANK_DESTROYER:
			return 97.5;
		default:
			throw;
	}
}

double Tank::mass() const {
	switch (type_) {
		case MEDIUM:
			return 10.0;
		case HEAVY:
			return 20.0;
		case TANK_DESTROYER:
			return 15.0;
		default:
			throw;
	}
}

double Tank::engine_power() const {
	switch (type_) {
		case MEDIUM:
			return 7500.0;
		case HEAVY:
			return 7500.0;
		case TANK_DESTROYER:
			return 5000.0;
		default:
			throw;
	}
}

double Tank::engine_rear_power_factor() const {
	switch (type_) {
		case MEDIUM:
			return 0.75;
		case HEAVY:
			return 0.5;
		case TANK_DESTROYER:
			return 0.35;
		default:
			throw;
	}
}

double Tank::turret_turn_speed() const {
	switch (type_) {
		case MEDIUM:
			return 1.0 * M_PI / 180.0;
		case HEAVY:
			return 0.5 * M_PI / 180.0;
		case TANK_DESTROYER:
			return 1.5 * M_PI / 180.0;
		default:
			throw;
	}
}

double Tank::turret_max_relative_angle() const {
	switch (type_) {
		case MEDIUM:
			return 0.0 * M_PI / 180.0;
		case HEAVY:
			return 0.0 * M_PI / 180.0;
		case TANK_DESTROYER:
			return 15.0 * M_PI / 180.0;
		default:
			throw;
	}
}

int Tank::crew_max_health() const {
	switch (type_) {
		case MEDIUM:
			return 100;
		case HEAVY:
			return 100;
		case TANK_DESTROYER:
			return 100;
		default:
			throw;
	}
}

int Tank::hull_max_durability() const {
	switch (type_) {
		case MEDIUM:
			return 200;
		case HEAVY:
			return 250;
		case TANK_DESTROYER:
			return 250;
		default:
			throw;
	}
}

int Tank::frontal_armor() const {
	switch (type_) {
		case MEDIUM:
			return 175;
		case HEAVY:
			return 200;
		case TANK_DESTROYER:
			return 250;
		default:
			throw;
	}
}

int Tank::side_armor() const {
	switch (type_) {
		case MEDIUM:
			return 150;
		case HEAVY:
			return 175;
		case TANK_DESTROYER:
			return 125;
		default:
			throw;
	}
}

int Tank::rear_armor() const {
	switch (type_) {
		case MEDIUM:
			return 100;
		case HEAVY:
			return 100;
		case TANK_DESTROYER:
			return 100;
		default:
			throw;
	}
}

double Tank::GetTurretAngleTo(double x, double y) const {
    double absolute_angle_to = atan2(y - this->y(), x - this->x());
    double relative_angle_to = absolute_angle_to - this->angle() - turret_relative_angle_;

    while (relative_angle_to > M_PI) {
        relative_angle_to -= 2.0 * M_PI;
    }

    while (relative_angle_to < -M_PI) {
        relative_angle_to += 2.0 * M_PI;
    }

    return relative_angle_to;
}

double Tank::GetTurretAngleTo(const Unit& unit) const {
    return this->GetTurretAngleTo(unit.x(), unit.y());
}
