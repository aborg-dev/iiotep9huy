#ifndef TANKCONTROLLER_HPP
#define TANKCONTROLLER_HPP

#include "Utility.hpp"
#include "Predictor.hpp"
#include "Geometry.hpp"

#include <iostream>

#define DB(x) cerr << #x << ": " << x << endl;

class TankController
{
public:
  TankController()
  {
  }
  TankController(Tank& self, Move& move)
  {
    this->self = &self;
    this->move = &move;
  }

  void updateMovePredictors()
  {
    vector<double> resultOutput;
    resultOutput.push_back(self->x());
    resultOutput.push_back(self->y());
    vector<double> predictedOutput = movePredictor.powerToMoveNetwork.getScaledOutput();

    /*
    std::cout << "predicted: " << predictedOutput[0] << " " << predictedOutput[1] << std::endl;
    std::cout << "result: " << resultOutput[0] << " " << resultOutput[1] << std::endl;

    double dx = predictedOutput[0] - resultOutput[0];
    double dy = predictedOutput[1] - resultOutput[1];
    std::cout << "difference: " << dx << " " << dy << std::endl;
    std::cout << "error: " << sqrt(dx*dx + dy*dy) << std::endl;
    */

    movePredictor.powerToMoveNetwork.train(resultOutput);
  }

  void updateTurretTurnPredictors()
  {
    vector<double> resultOutput;

    resultOutput.push_back(-self->GetTurretAngleTo(self->x() + 1, self->y()));

    //vector<double> predictedOutput = turretTurnPredictor.turretTurnNetwork.getScaledOutput();

    vector<double> predictedOutput(1, previousPredictedAngle);

    std::cout << "predicted: " << predictedOutput[0] << std::endl;
    std::cout << "result: " << resultOutput[0] << std::endl;

    double da = predictedOutput[0] - resultOutput[0];
    std::cout << "difference: " << da << std::endl;

    //turretTurnPredictor.trainTurretTurnNetwork(resultOutput);
  }

  void updatePredictors()
  {
    updateMovePredictors();
    updateTurretTurnPredictors();
  }

  bool isCorrectTurretDirection(const Tank& tank)
  {
    Tank& self = *(this->self);

    double angle = self.GetTurretAngleTo(tank);
    double distance = self.GetDistanceTo(tank);
    double side = (tank.width() + tank.height()) / 2.0;
    double minAngle = fabs(atan(side / (2 * distance)));

    return (fabs(angle) < minAngle * 0.8);
  }

  double getPredictedAngle(double turn)
  {
    Tank& self = *(this->self);
    Move& move = *(this->move);

    double curTurrentAngle = -self.GetTurretAngleTo(self.x() + 1, self.y());
    double turnSpeed = self.turret_turn_speed();

    double crewEff = (self.crew_health() + 100.0) / 200.0;
    double crew = self.crew_health();
    cerr << "crew: " << crew << endl;
    cerr << "turn: " << turn << endl;
    if (turn > 0)
      turn = 1.0;
    else
    if (turn < 0)
      turn = -1.0;
    else
      turn = 0.0;

    //if (angle > 0)
      return curTurrentAngle + turn * turnSpeed * crewEff + self.angular_speed();
    //else
      //return curTurrentAngle - turn * turnSpeed + self.angular_speed();
  }

  void setTurrentDirection(const Tank& tank)
  {
    Tank& self = *(this->self);
    Move& move = *(this->move);

    //Vector2f target = getPredictedPosition(tank);
    //double targetAngle = self.GetTurretAngleTo(target.x, target.y);
    //double targetAngle = self.GetTurretAngleTo(tank);
    long double y0 = self.y();
    long double x0 = self.x();
    long double y = tank.y();
    long double x = tank.x();

    long double alpha = atan2(x - x0, y - y0);
    DB(x);
    DB(y);
    DB(y0);
    DB(x0);
    DB((alpha / M_PI) * 180);
    long double targetAngle = 0.0;
    //long double gamma = self.angle() + self.turret_relative_angle();
    //long double targetAngle = gamma - alpha;
    //targetAngle += self.angular_speed();

    //double targetAngle = predictAngle(self, tank);
    //double turrentAngle = self.turret_relative_angle() + self.angle();

    //DB(((targetAngle) / M_PI) * 180);
    //DB(((targetAngle - turrentAngle) / M_PI) * 180);
    DB((targetAngle / M_PI) * 180);

    if (targetAngle > 0)
    {
      DB("RIGHT");
      setTurretTurn(1.0);
    }
    else
    if (targetAngle < 0)
    {
      DB("LEFT");
      setTurretTurn(-1.0);
    }
    else
    {
      DB("SAME");
      setTurretTurn(0.0);
    }
  }

  double getSpeed()
  {
    return getDistance(Vector2f(0, 0), Vector2f(self->speed_x(), self->speed_y()));
  }

  long double sqr(long double x)
  {
    return x * x;
  }

  double predictAngle(const Tank& self, const Tank& target)
  {
    long double x = target.x();
    long double y = target.y();
    long double vx = target.speed_x();
    long double vy = target.speed_y();
    DB(vx);
    DB(vy);
    long double x0 = self.x();
    long double y0 = self.y();
    long double v1 = 16.7;

    double f = (v1 * (x-x0) * (vx * (y0-y)+vy * (x-x0)) -
                  sqrtl(fabsl(
                  sqr(v1) * sqr(y-y0) * (sqr(v1) * (sqr(x - x0) + sqr(y-y0)) -
                  sqr(vx * (y0-y) + vy * (x-x0)))
                ))
                ) / (sqr(v1) * (sqr(x - x0) + sqr(y-y0)));

    return asinl(f);
    //return asin((v1 * (x-x0) * (vx * (y0-y)+vy * (x-x0)) -
            //sqrt(sqr(v1) * sqr(y-y0) * (sqr(v1) * (sqr(x) - 2 * x * x0 + sqr(x0) + sqr(y-y0)) -
            //sqr(vx * (y0-y) + vy * (x-x0))))) / (sqr(v1) * (sqr(x) - 2 * x * x0 + sqr(x0) + sqr(y-y0))));
  }

//(v1 (x-x0) (vx (y0-y)+vy (x-x0))-Sqrt[v1^2 (y-y0)^2 (v1^2 (x^2-2 x x0+x0^2+(y-y0)^2)
//-(vx (y0-y)+vy (x-x0))^2)])/(v1^2 (x^2-2 x x0+x0^2+(y-y0)^2))


  Vector2f getPredictedPosition(const Tank& targetTank)
  {
    Tank& self = *(this->self);

    Vector2f position(targetTank);

    double distance = getDistance(self, targetTank);
    double bulletFlyTime = getBulletFlyTime(distance, getSpeed()) / 4;

    position.x += targetTank.speed_x() * bulletFlyTime;
    position.y += targetTank.speed_y() * bulletFlyTime;
    return position;
  }

  bool targetEnemy(const Tank& targetTank)
  {
    setTurrentDirection(targetTank);
    if (isCorrectTurretDirection(targetTank))
      fire(targetTank);
  }

  void moveToPosition(Vector2f& targetPosition) {
    Tank& self = *(this->self);
    Move& move = *(this->move);

    double targetAngle = self.GetAngleTo(targetPosition.x, targetPosition.y);
    double distance = self.GetDistanceTo(targetPosition.x, targetPosition.y);

    double rearFactor = self.engine_rear_power_factor();
    double c = moveCoefficient(targetAngle);

    //std::cerr << targetPosition.x << " " << targetPosition.y << std::endl;
    //DB(distance);
    if (!manyMoveTargetsMode && distance < 120)
    {
      //DB(targetAngle);
      if (fabs(targetAngle) < MIN_ROTATE_ANGLE)
      {
        setTrackPowers(1.0, 1.0);
      }
      else
      if (M_PI - fabs(targetAngle) < MIN_ROTATE_ANGLE)
      {
        setTrackPowers(-1.0, -1.0);
      }
      else
      if (targetAngle > 0)
      {
        setTrackPowers(-1.0 * c, 1.0 * rearFactor);
      }
      else
      if (targetAngle < 0)
      {
        setTrackPowers(1.0 * rearFactor, -1.0 * c);
      }
    }
    else
    if (fabs(targetAngle) < (2 * M_PI / 3))
    {
      targetAngle /= (2*M_PI / 3);
      setTrackPowers(1.0 + targetAngle * 2.0, 1.0 - targetAngle * 2.0);
    }
    else
    if (fabs(targetAngle) > (2*M_PI / 3))
    {
      targetAngle /= (2*M_PI / 3);
      if (targetAngle > 0)
        targetAngle = 2 - targetAngle;
      else
        targetAngle = -2 - targetAngle;

      setTrackPowers(-1.0 - targetAngle * 2.0, -1.0 + targetAngle * 2.0);
    }
    else
    if (targetAngle < 0)
    {
      //DB("LEFT");
      setTrackPowers(-1.0 * c, 1.0 * rearFactor);
    }
    else
    if (targetAngle > 0)
    {
      //DB("RIGHT");
      setTrackPowers(1.0 * rearFactor, -1.0 * c);
    }
    previousAngularSpeed = self.angular_speed();
  }

  void fire(const Tank& targetTank)
  {
    setFireType(PREMIUM_PREFERRED);
  }

  void setFireType(model::FireType fireType)
  {
    move->set_fire_type(fireType);
  }

  void setLeftTrackPower(double leftPower)
  {
    move->set_left_track_power(leftPower);
  }

  void setRightTrackPower(double rightPower)
  {
    move->set_right_track_power(rightPower);
  }

  double calculateCorrection(double leftPower, double rightPower)
  {
    double correction = getTurrentTurn();
    if (leftPower < 0 && rightPower > 0)
    {
      if (getTurrentTurn() < 0)
      {
        correction = -correction / 2;
      }
    }
    else
    if (leftPower > 0 && rightPower < 0)
    {
      if (getTurrentTurn() > 0)
      {
        correction = -correction / 2;
      }
    }
    return correction;
  }

  void correctTurretAngle(double correction)
  {
    setTurretTurn(correction);
  }

  void setTrackPowers(double leftPower, double rightPower)
  {
    setLeftTrackPower(leftPower);
    setRightTrackPower(rightPower);

    movePredictor.powerToMoveNetwork.predict(leftPower, rightPower, *self);
    //correctTurretAngle(calculateCorrection(leftPower, rightPower));
  }

  void setTurretTurn(double turn)
  {
    move->set_turret_turn(turn);
    //turretTurnPredictor.predictTurretTurn(turn, *move, *self);
  }

  double getTurrentTurn()
  {
    return move->turret_turn();
  }

  double previousAngularSpeed;
  double previousPredictedAngle;
  bool manyMoveTargetsMode;
  Tank* self;
  Move* move;

  MovePredictor movePredictor;
  TurretTurnPredictor turretTurnPredictor;
};

#endif // TANKCONTROLLER_HPP
