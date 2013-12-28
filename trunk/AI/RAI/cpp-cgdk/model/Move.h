#pragma once

#ifndef _MOVE_H_
#define _MOVE_H_

namespace model {
	enum FireType {
		UNKNOWN_FIRE = -1,
		NONE = 0,
		REGULAR_FIRE = 1,
		PREMIUM_FIRE = 2,
		PREMIUM_PREFERRED = 3
	};

	class Move {
	private:
		double left_track_power_;
		double right_track_power_;
		double turret_turn_;
		FireType fire_type_;
	public:
        Move();
    
		double left_track_power() const;
		double right_track_power() const;
		double turret_turn() const;
		FireType fire_type() const;

		void set_left_track_power(double left_track_power);
		void set_right_track_power(double right_track_power);
		void set_turret_turn(double turret_turn);
		void set_fire_type(FireType fire_type);
	};
}

#endif
