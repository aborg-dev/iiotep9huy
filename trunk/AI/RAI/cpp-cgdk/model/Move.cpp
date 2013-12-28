#include "Move.h"

using namespace model;

Move::Move() : left_track_power_(0.0), right_track_power_(0.0), turret_turn_(0.0), fire_type_(NONE) {
}

double Move::left_track_power() const {
	return left_track_power_;
}

double Move::right_track_power() const {
	return right_track_power_;
}

double Move::turret_turn() const {
	return turret_turn_;
}

FireType Move::fire_type() const {
	return fire_type_;
}

void Move::set_left_track_power(double left_track_power) {
	left_track_power_ = left_track_power;
}

void Move::set_right_track_power(double right_track_power) {
	right_track_power_ = right_track_power;
}

void Move::set_turret_turn(double turret_turn) {
	turret_turn_ = turret_turn;
}

void Move::set_fire_type(FireType fire_type) {
	fire_type_ = fire_type;
}
