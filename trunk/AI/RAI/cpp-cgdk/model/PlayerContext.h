#pragma once

#ifndef _PLAYER_CONTEXT_H_
#define _PLAYER_CONTEXT_H_

#include <vector>

#include "Tank.h"
#include "World.h"

namespace model {
	class PlayerContext {
	private:
		std::vector<Tank> tanks_;
		World world_;
	public:
		PlayerContext(const std::vector<Tank>& tanks, World world);

		std::vector<Tank> tanks() const;
		World world() const;
	};
}

#endif
