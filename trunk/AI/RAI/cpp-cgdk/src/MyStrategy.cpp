#include "MyStrategy.h"

#include "TankObjections.hpp"
#include "Utility.hpp"
#include "Geometry.hpp"
#include "MyWorld.hpp"
#include "MyTank.hpp"
#include "NeuralNetwork.hpp"
#include "Utility.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>

using namespace model;

using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::make_pair;

#define DB(x) cerr << #x << ": " << x << endl;

MyWorld myWorld;

void MyStrategy::Move(Tank self, World world, model::Move& move) {

  if (myWorld.tickNumber == 0)
  {
    myWorld.initWorld(self, move, world);
  }
  else
  {
    myWorld.updateWorld(self, move, world);
  }

  MyTank& myTank = myWorld.getTank(self.id());
  if (myWorld.tickNumber == 0)
  {
    //myTank.controller.movePredictor.powerToMoveNetwork.read("powerToMoveNetwork.nn");
  }

  //myTank.controller.updatePredictors();

  myTank.controller.setFireType(NONE);
  myTank.controller.setTrackPowers(-1.0, -1.0);
  myTank.controller.setTurretTurn(0.0);

  //myTank.controller.movePredictor.powerToMoveNetwork.save("powerToMoveNetwork.nn");

  //myTank.controller.setTrackPowers(1.0, 1.0);
  //if (world.tick()
  myTank.setObjections();
  myTank.completeObjections();
}

TankType MyStrategy::SelectTank(int tank_index, int team_size) {
  //return HEAVY;
  return MEDIUM;
}
