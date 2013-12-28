#pragma once

#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include "NeuralNetwork.hpp"

class PredictNetwork
{
public:
  PredictNetwork()
  {
  }

  void save(string fileName)
  {
    network.save(fileName);
  }
  void read(string fileName)
  {
    network.read(fileName);
  }

  Network network;
};

class TurretTurnNetwork : public PredictNetwork
{
public:
  TurretTurnNetwork()
  {
    vector<size_t> layersSize(4);
    layersSize[0] = 6;
    layersSize[1] = 5;
    layersSize[2] = 4;
    layersSize[3] = 1;
    network.setNetworkParams(0.7, layersSize);
    network.randomizeDendrites();
  }

  vector<double> scaleOutput(vector<double> output, bool invert)
  {
    if (invert == 0)
    {
      output[0] *= 2 * M_PI;
      output[0] -= M_PI;
    }
    else
    {
      output[0] += M_PI;
      output[0] /= 2 * M_PI;
    }
    return output;
  }

  vector<double> predict(double turretTurn, const Move& move, const Tank& tank)
  {
    vector<double> input;
    input.push_back(turretTurn);
    input.push_back(tank.angular_speed());
    input.push_back(tank.speed_x());
    input.push_back(tank.speed_y());
    input.push_back(tank.angular_speed());
    input.push_back(tank.angle());
    /*input.push_back(move.left_track_power());
    input.push_back(move.right_track_power());
    input.push_back(tank.turret_turn_speed());
    input.push_back(tank.turret_relative_angle());
    input.push_back(tank.turret_max_relative_angle());
    input.push_back(tank.angle());
    input.push_back(tank.angular_speed());
    input.push_back(tank.x());
    input.push_back(tank.y());
    input.push_back(tank.speed_x());
    input.push_back(tank.speed_y());
    input.push_back(tank.crew_health());
    input.push_back(tank.engine_power());
    input.push_back(tank.mass());
    input.push_back(tank.engine_rear_power_factor());
    input.push_back(tank.width());
    input.push_back(tank.height());*/

    network.setInputs(input);
    return scaleOutput(network.getOutput(), false);
  }

  void train(vector<double> resultOutput)
  {
    network.doBackPropagation(scaleOutput(resultOutput, true));
  }

  vector<double> getScaledOutput()
  {
    return scaleOutput(network.getReadyOutput(), false);
  }

};

class PowerToMoveNetwork : public PredictNetwork
{
public:
  PowerToMoveNetwork()
  {
    vector<size_t> layersSize(4);
    layersSize[0] = 14;
    layersSize[1] = 20;
    layersSize[2] = 20;
    layersSize[3] = 2;
    network.setNetworkParams(0.7, layersSize);
    network.randomizeDendrites();
  }

  vector<double> scaleOutput(vector<double> output, bool invert)
  {
    if (invert == 0)
    {
      output[0] *= 1280;
      output[1] *= 800;
    }
    else
    {
      output[0] /= 1280;
      output[1] /= 800;
    }
    return output;
  }

  vector<double> predict(double leftPower, double rightPower, const Tank& tank)
  {
    vector<double> input;
    input.push_back(leftPower);
    input.push_back(rightPower);
    input.push_back(tank.angle());
    input.push_back(tank.angular_speed());
    input.push_back(tank.x());
    input.push_back(tank.y());
    input.push_back(tank.speed_x());
    input.push_back(tank.speed_y());
    input.push_back(tank.crew_health());
    input.push_back(tank.engine_power());
    input.push_back(tank.mass());
    input.push_back(tank.engine_rear_power_factor());
    input.push_back(tank.width());
    input.push_back(tank.height());

    network.setInputs(input);
    return scaleOutput(network.getOutput(), false);
  }

  void train(vector<double> resultOutput)
  {
    network.doBackPropagation(scaleOutput(resultOutput, true));
  }

  vector<double> getScaledOutput()
  {
    return scaleOutput(network.getReadyOutput(), false);
  }
};

class MoveToPowerNetwork : public PredictNetwork
{
public:
  MoveToPowerNetwork()
  {
    vector<size_t> layersSize(4);
    layersSize[0] = 12;
    layersSize[1] = 20;
    layersSize[2] = 20;
    layersSize[3] = 2;
    network.setNetworkParams(0.7, layersSize);
    network.randomizeDendrites();
  }

  vector<double> scaleOutput(vector<double> output, bool invert)
  {
    return output;
  }

  vector<double> predict(double leftPower, double rightPower, const Tank& tank)
  {
    vector<double> input;
    input.push_back(tank.x());
    input.push_back(tank.y());
    input.push_back(tank.angle());
    input.push_back(tank.angular_speed());
    input.push_back(tank.speed_x());
    input.push_back(tank.speed_y());
    input.push_back(tank.crew_health());
    input.push_back(tank.engine_power());
    input.push_back(tank.mass());
    input.push_back(tank.engine_rear_power_factor());
    input.push_back(tank.width());
    input.push_back(tank.height());

    network.setInputs(input);
    return scaleOutput(network.getOutput(), false);
  }

  void train(vector<double> resultOutput)
  {
    network.doBackPropagation(scaleOutput(resultOutput, true));
  }

  vector<double> getScaledOutput()
  {
    return scaleOutput(network.getReadyOutput(), false);
  }
};

class MovePredictor
{
public:

  MovePredictor()
  {
    // Params: leftPower, rightPower, angle, angularSpeed, speed_x, speed_y, crewHealt, enginePower, mass, engineRearPowerFactor,
    // width, heigth
    // Output: x, y; In future add speed_x, speed_y, angle, angularSpeed
  }

  vector<double> predictPowerToMove(double leftPower, double rightPower, const Tank& tank)
  {
    return powerToMoveNetwork.predict(leftPower, rightPower, tank);
  }

  vector<double> predictMoveToPower(double x, double y, const Tank& tank)
  {
    return moveToPowerNetwork.predict(x, y, tank);
  }

  void trainPowerToMoveNetwork(vector<double> resultOutput)
  {
    powerToMoveNetwork.train(resultOutput);
  }

  PowerToMoveNetwork powerToMoveNetwork;
  MoveToPowerNetwork moveToPowerNetwork;
};

class TurretTurnPredictor
{
public:
  TurretTurnPredictor()
  {
  }

  vector<double> predictTurretTurn(double turretTurn, const Move& move, const Tank& tank)
  {
    return turretTurnNetwork.predict(turretTurn, move, tank);
  }

  void trainTurretTurnNetwork(vector<double> resultOutput)
  {
    turretTurnNetwork.train(resultOutput);
  }

  TurretTurnNetwork turretTurnNetwork;
};

#endif // PREDICTOR_HPP
