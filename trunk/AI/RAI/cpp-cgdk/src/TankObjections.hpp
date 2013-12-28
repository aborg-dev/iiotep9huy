#pragma once

#ifndef TANKOBJECTIONS_HPP
#define TANKOBJECTIONS_HPP

#include "MyStrategy.h"
#include "Geometry.hpp"

using namespace model;

class TankObjections
{
public:
  TankObjections()
  {
    targetTank = 0;
    targetPosition.x = 0;
    targetPosition.y = 0;
  }

  void setTargetTank(Tank* tank)
  {
    targetTank = tank;
  }

  void setTargetPosition(const Vector2f& position)
  {
    moveObjectionFlag = true;
    targetPosition = position;
  }

  void setTargetBonus(Bonus* bonus)
  {
    targetBonus = bonus;
    setTargetPosition(Vector2f(bonus->x(), bonus->y()));
  }

  bool haveMoveObjection()
  {
    return moveObjectionFlag;
  }

  bool haveTarget()
  {
    return (targetTank);
  }

  Tank* targetTank;
  Vector2f targetPosition;
  Bonus* targetBonus;
  bool moveObjectionFlag;
};

#endif // TANKOBJECTIONS_HPP
