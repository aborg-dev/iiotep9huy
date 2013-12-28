#pragma once

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <map>
#include <cstdlib>

#include "MyStrategy.h"
#include "Geometry.hpp"

using namespace model;
using std::map;
using std::swap;
using std::min;

const double MIN_ANGLE = M_PI / 180.0;
const double MIN_ROTATE_ANGLE = 0.4;
const double BULLET_SPEED = 16.7;

double myRand()
{
  return double(rand()) / (RAND_MAX);
}

double myRandSigned()
{
  double x = myRand();
  return rand()&1 ? x : -x;
}

bool isAlive(const Tank& tank)
{
  return (tank.hull_durability() > 0 && tank.crew_health() > 0);
}

double getBulletSpeed(double distance)
{
  return BULLET_SPEED;
}

double getBulletFlyTime(double distance, double initialSpeed)
{
  return distance / (getBulletSpeed(distance) + initialSpeed);
}

double normalize(double angle)
{
  if (angle > M_PI)
    angle -= 2 * M_PI;

  if (angle < -M_PI)
    angle += 2 * M_PI;

  return angle;
}

double getShortestAngle(double sAngle, double fAngle, double turn)
{
  double rot = fAngle - sAngle;
  if (rot * turn > 0)
  {
    return -normalize(rot);
  }
  else
  {
    return normalize(rot);
  }
}

Vector2f upLeft(0.0, 0.0);
Vector2f upRight(0.0, 1280);
Vector2f downLeft(800, 1280);
Vector2f downRight(800, 0);

Vector2f getNearestCorner(const Vector2f& position)
{
  Vector2f closest = upLeft;
  if (getDistance(closest, position) > getDistance(upRight, position))
    closest = upRight;

  if (getDistance(closest, position) > getDistance(downLeft, position))
    closest = downLeft;

  if (getDistance(closest, position) > getDistance(downRight, position))
    closest = downRight;

  return closest;
}

double moveCoefficient(double angle)
{
  angle = fabs(angle);
  return angle / 1.5;
}

class Config
{
public:
  Config()
  {
    bonusUtility[MEDIKIT] = 0.8;
    bonusUtility[REPAIR_KIT] = 0.9;
    bonusUtility[AMMO_CRATE] = 1.0;
  }

  map<BonusType, double> bonusUtility;
};

Config defaultConfig;

#endif // UTILITY_HPP
