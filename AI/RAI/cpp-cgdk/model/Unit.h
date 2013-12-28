#pragma once

#ifndef _UNIT_H_
#define _UNIT_H_

namespace model {
	class Unit {
	private:
		long long id_;
		double width_;
		double height_;
		double x_;
		double y_;
		double speed_x_;
		double speed_y_;
		double angle_;
		double angular_speed_;
	protected:
		Unit(long long id, double width, double height, double x, double y,
			double speed_x, double speed_y, double angle, double angular_speed);
	public:
		virtual ~Unit();

		long long id() const;
		double width() const;
		double height() const;
		double x() const;
		double y() const;
		double speed_x() const;
		double speed_y() const;
		double angle() const;
		double angular_speed() const;

		double GetAngleTo(double x, double y) const;
		double GetAngleTo(const Unit& unit) const;
		double GetDistanceTo(double x, double y) const;
		double GetDistanceTo(const Unit& unit) const;
	};
}

#endif
