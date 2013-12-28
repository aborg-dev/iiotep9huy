#pragma once

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>

#include "MyStrategy.h"

using namespace model;

const double eps = 1e-9;

struct Vector2f
{
  Vector2f(double x = 0.0, double y = 0.0) : x(x), y(y)
  {
  }
  Vector2f(const Unit& unit)
  {
    x = unit.x();
    y = unit.y();
  }
  double x, y;
};

struct Segment
{
  Vector2f a, b;
};

bool equal(double x, double y)
{
  return (fabs(x - y) < eps);
}

double getDistance(const Vector2f& a, const Vector2f& b)
{
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool onSegment(const Vector2f& point, const Segment& segment)
{
  return equal(getDistance(segment.a, point) + getDistance(segment.b, point),
               getDistance(segment.a, segment.b));
}

bool intersectSegments(const Segment& s1, const Segment& s2, Vector2f& intersection)
{

}

#endif // GEOMETRY_HPP
