#ifndef MYWORLD_HPP
#define MYWORLD_HPP

#include <map>

#include "MyTank.hpp"
#include "MyStrategy.h"

using namespace model;
using std::map;

class MyWorld
{
public:
  MyWorld()
  {
    tickNumber = 0;
  }

  void initWorld(Tank& self, Move& move, World& world)
  {
    addTank(self, move, world);
    tickNumber = world.tick();
  }

  void updateWorld(Tank& self, Move& move, World& world)
  {
    tanks[self.id()].self = &self;
    tanks[self.id()].move = &move;
    tanks[self.id()].world = &world;
    tickNumber = world.tick();
  }

  void addTank(Tank& tank, Move& move, World& world)
  {
    tanks[tank.id()] = MyTank(tank, move, world);
  }

  MyTank& getTank(long long id)
  {
    return tanks[id];
  }

  map<long long, MyTank> tanks;
  int tickNumber;
};

#endif // MYWORLD_HPP
