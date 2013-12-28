#include "Unit.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace model;

Unit::Unit(long long id, double width, double height, double x, double y,
		   double speed_x, double speed_y, double angle, double angular_speed)
: id_(id), width_(width), height_(height), x_(x), y_(y),
speed_x_(speed_x), speed_y_(speed_y), angle_(angle), angular_speed_(angular_speed) { }

Unit::~Unit() { }

long long Unit::id() const {
	return id_;
}

double Unit::width() const {
	return width_;
}

double Unit::height() const {
	return height_;
}	

double Unit::x() const {
	return x_;
}

double Unit::y() const {
	return y_;
}

double Unit::speed_x() const {
	return speed_x_;
}

double Unit::speed_y() const {
	return speed_y_;
}

double Unit::angle() const {
	return angle_;
}

double Unit::angular_speed() const {
	return angular_speed_;
}

double Unit::GetAngleTo(double x, double y) const {
	double absolute_angle_to = atan2(y - this->y_, x - this->x_);
    double relative_angle_to = absolute_angle_to - this->angle_;

    while (relative_angle_to > M_PI) {
		relative_angle_to -= 2.0 * M_PI;
	}

    while (relative_angle_to < -M_PI) {
		relative_angle_to += 2.0 * M_PI;
	}

    return relative_angle_to;
}

double Unit::GetAngleTo(const Unit& unit) const {
    return this->GetAngleTo(unit.x(), unit.y());
}

double Unit::GetDistanceTo(double x, double y) const {
    return hypot(x - this->x_, y - this->y_);
}

double Unit::GetDistanceTo(const Unit& unit) const {
    return this->GetDistanceTo(unit.x(), unit.y());
}
