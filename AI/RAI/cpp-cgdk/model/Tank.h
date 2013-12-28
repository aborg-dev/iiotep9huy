#pragma once

#ifndef _TANK_H_
#define _TANK_H_

#define _USE_MATH_DEFINES
#include <string>

#include "Unit.h"

namespace model {
	enum TankType {
		UNKNOWN_TANK = -1,
		MEDIUM = 0,
		HEAVY = 1,
		TANK_DESTROYER = 2
	};

	class Tank : public Unit {
	private:
		std::string player_name_;
		int teammate_index_;
		double turret_relative_angle_;
		int crew_health_;
		int hull_durability_;
		int reloading_time_;
		int remaining_reloading_time_;
		int premium_shell_count_;
		bool teammate_;
		TankType type_;

		double GetWidth(const TankType type) const;
		double GetHeight(const TankType type) const;
	public:
		Tank(long long id, std::string player_name, int teammate_index,
			double x, double y, double speed_x, double speed_y, double angle, double angular_speed,
			double turret_relative_angle, int crew_health, int hull_durability,
			int reloading_time, int remaining_reloading_time, int premium_shell_count, bool teammate, TankType type);

		std::string player_name() const;
		int teammate_index() const;
		double turret_relative_angle() const;
		int crew_health() const;
		int hull_durability() const;
		int reloading_time() const;
		int remaining_reloading_time() const;
		int premium_shell_count() const;
		bool teammate() const;
		TankType type() const;

		double virtual_gun_length() const;
		double mass() const;
		double engine_power() const;
		double engine_rear_power_factor() const;
		double turret_turn_speed() const;
		double turret_max_relative_angle() const;
		int crew_max_health() const;
		int hull_max_durability() const;
		int frontal_armor() const;
		int side_armor() const;
		int rear_armor() const;

		double GetTurretAngleTo(double x, double y) const;
		double GetTurretAngleTo(const Unit& unit) const;
	};
}

#endif
