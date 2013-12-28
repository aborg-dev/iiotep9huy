#include "PlayerContext.h"

using namespace model;
using namespace std;

PlayerContext::PlayerContext(const vector<Tank>& tanks, World world)
: tanks_(tanks), world_(world) { }

vector<Tank> PlayerContext::tanks() const {
	return tanks_;
}

World PlayerContext::world() const {
	return world_;
}
