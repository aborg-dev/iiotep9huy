#include "Bonus.h"

using namespace model;

Bonus::Bonus(long long id, double width, double height, double x, double y, BonusType type)
: Unit(id, width, height, x, y, 0.0, 0.0, 0.0, 0.0), type_(type) { }

BonusType Bonus::type() const {
	return type_;
}
