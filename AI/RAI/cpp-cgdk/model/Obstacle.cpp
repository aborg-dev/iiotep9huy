#include "Obstacle.h"

using namespace model;

Obstacle::Obstacle(long long id, double width, double height, double x, double y)
: Unit(id, width, height, x, y, 0.0, 0.0, 0.0, 0.0) { }
