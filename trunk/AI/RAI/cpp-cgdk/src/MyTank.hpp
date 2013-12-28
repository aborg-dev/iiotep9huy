#ifndef MYTANK_HPP
#define MYTANK_HPP

#include "TankObjections.hpp"
#include "TankController.hpp"

using namespace model;

class MyTank
{
public:
  MyTank()
  {
    prevTurretAngle = 0.0;
  }
  MyTank(Tank& self, Move& move, World& world) : controller(self, move), id(self.id())
  {
    this->self = &self;
    this->move = &move;
    this->world = &world;
  }

  Bonus& findNearestBonus()
  {
    double bestDistance = 1e9; int bestBonusIndex; for (int i = 0; i < world->bonuses().size(); ++i)
    {
      Bonus& bonus = world->bonuses()[i];
      double distance = self->GetDistanceTo(bonus) * defaultConfig.bonusUtility[bonus.type()];
      if (distance * 2 > bestDistance && fabs(self->GetAngleTo(bonus)) > MIN_ROTATE_ANGLE)
        continue;
      if (distance < bestDistance)
      {
        if (fabs(distance - bestDistance) < 40)
          controller.manyMoveTargetsMode = true;
        bestDistance = distance;
        bestBonusIndex = i;
      }
    }
    return world->bonuses()[bestBonusIndex];
  }

  Bonus* findBonus()
  {
    Bonus* bestBonus = 0;
    if (!world->bonuses().empty())
    {
      bestBonus = &findNearestBonus();
    }
    return bestBonus;
  }

  Tank* findEnemyByHealth()
  {
    double minHealth = 1e20;
    Tank* bestTarget = 0;

    for (int i = 0; i < world->tanks().size(); ++i)
    {
      Tank& tank = world->tanks()[i];
      if (tank.teammate())
        continue;
      if (!isAlive(tank))
        continue;

      if (tank.hull_durability() < minHealth)
      {
        minHealth = tank.hull_durability();
        bestTarget = &tank;
      }
    }
    return bestTarget;
  }

  Tank* findEnemyByAngle()
  {
    double minAngle = 1e20;
    Tank* bestTarget = 0;

    for (int i = 0; i < world->tanks().size(); ++i)
    {
      Tank& tank = world->tanks()[i];
      if (tank.teammate())
        continue;
      if (!isAlive(tank))
        continue;

      if (fabs(self->GetAngleTo(tank)) < minAngle)
      {
        minAngle = fabs(self->GetAngleTo(tank));
        bestTarget = &tank;
      }
    }
    return bestTarget;
  }

  void targetEnemy()
  {
    if (objections.haveTarget())
    {
      controller.targetEnemy(*objections.targetTank);
    }
  }

  void moveToObjection()
  {
    if (objections.haveMoveObjection())
    {
      controller.moveToPosition(objections.targetPosition);
    }
  }

  void setObjections()
  {
    controller.manyMoveTargetsMode = false;
    if (Tank* enemy = findEnemyByHealth())
      objections.setTargetTank(enemy);

    //if (Tank* enemy = findEnemyByAngle())
      //objections.setTargetTank(enemy);

    //std::cerr << self->x() << " " << self->y() << std::endl;
    //std::cerr << world->tick() << std::endl;
    if (world->tick() < 150)
    {
      objections.setTargetPosition(getNearestCorner(*self));
      //objections.setTargetPosition(Vector2f(1000, 1000));
    }
    else
    {
      if (Bonus* bonus = findBonus())
        objections.setTargetBonus(bonus);
    }
  }

  void completeObjections()
  {
    //moveToObjection();
    /*if (world->tick() < 100)
    {
      double turn = 0.1 * world->tick();
      controller.setTurretTurn(turn);
      double angle = self->turret_relative_angle();
      cerr << "turn: " << turn << endl;
      cerr << "angle: " << angle << endl;
      cerr << "diff: " << angle - prevTurretAngle << endl;
      cerr << endl;
      prevTurretAngle = angle;
    }*/
    targetEnemy();
  }

  double prevTurretAngle;

  Tank* self;
  Move* move;
  World* world;
  TankObjections objections;
  TankController controller;
  long long id;
};

#endif // MYTANK_HPP
