#include "Player.h"

using namespace model;
using namespace std;

Player::Player(string name, int score, bool strategy_crashed)
: name_(name), score_(score), strategy_crashed_(strategy_crashed) { }

string Player::name() const {
	return name_;
}

int Player::score() const {
	return score_;
}

bool Player::strategy_crashed() const {
	return strategy_crashed_;
}
