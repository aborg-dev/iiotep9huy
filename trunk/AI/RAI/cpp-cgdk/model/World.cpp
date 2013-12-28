#include "World.h"

using namespace model;
using namespace std;

World::World(int tick, double width, double height, const vector<Player>& players, const vector<Obstacle>& obstacles,
			 const vector<Tank>& tanks, const vector<Shell>& shells, const vector<Bonus>& bonuses)
			 : tick_(tick), width_(width), height_(height), players_(players), tanks_(tanks),
			 shells_(shells), bonuses_(bonuses) { }

int World::tick() const {
	return tick_;
}

double World::width() const {
	return width_;
}

double World::height() const {
	return height_;
}

vector<Player> World::players() const {
	return players_;
}

vector<Obstacle> World::obstacles() const {
	return obstacles_;
}

vector<Tank> World::tanks() const {
	return tanks_;
}

vector<Shell> World::shells() const {
	return shells_;
}

vector<Bonus> World::bonuses() const {
	return bonuses_;
}
