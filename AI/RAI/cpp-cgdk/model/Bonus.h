#pragma once

#ifndef _BONUS_H_
#define _BONUS_H_

#include "Unit.h"

namespace model {
	enum BonusType {
		UNKNOWN_BONUS = -1,
		MEDIKIT = 0,
		REPAIR_KIT = 1,
		AMMO_CRATE = 2
	};

	class Bonus : public Unit {
	private:
		BonusType type_;
	public:
		Bonus(long long id, double width, double height, double x, double y, BonusType type);

		BonusType type() const;
	};
}

#endif
