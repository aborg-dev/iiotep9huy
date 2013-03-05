#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <memory>
#include <iomanip>
#include <limits>
#include <math.h>

struct Point
{
  Point(int xCoord, int yCoord): xCoord(xCoord), yCoord(yCoord) {}

  Point(): Point(0, 0) {}

  int xCoord, yCoord;
};

std::istream& operator>> (std::istream& stream, Point& point)
{
  stream >> point.xCoord >> point.yCoord;
  return stream;
}

std::ostream& operator<< (std::ostream& stream, const Point& point)
{
  stream << point.xCoord << " " << point.yCoord;
  return stream;
}

double sqr(double value)
{
  return value * value;
}

double getDistance(const Point& firstPoint, const Point& secondPoint)
{
  return sqrt(sqr(firstPoint.xCoord - secondPoint.xCoord) 
            + sqr(firstPoint.yCoord - secondPoint.yCoord)); 
}

void readInput(std::vector<Point>* agentPositions)
{
  int agentsNumber;
  std::cin >> agentsNumber;

  agentPositions->resize(agentsNumber);

  for (int i = 0; i < agentsNumber; ++i)
  {
    std::cin >> (*agentPositions)[i];
  }
}

double findMinimalSignalRadius(const std::vector<Point>& agentPositions)
{
  size_t agentsNumber = agentPositions.size();

  const int NO_AGENT = -1;
  const double NO_DISTANCE = std::numeric_limits<double>::max();

  std::vector<double> shortestDistanceToAgent(agentsNumber, NO_DISTANCE);

  std::vector<bool> agentIsUsed(agentsNumber);

  shortestDistanceToAgent[0] = 0.0;
  double minimalSignalRadius = 0.0;

  for (int iteration = 0; iteration < agentsNumber; ++iteration)
  {
    double shortestDistance = NO_DISTANCE;
    int closestAgentIndex = NO_AGENT;

    for (int agentIndex = 0; agentIndex < agentsNumber; ++agentIndex)
    {
      if (agentIsUsed[agentIndex] || shortestDistanceToAgent[agentIndex] == NO_DISTANCE)
      {
        continue;
      }

      
      if (closestAgentIndex == NO_AGENT || 
          shortestDistanceToAgent[agentIndex] < shortestDistance)
      {
        shortestDistance = shortestDistanceToAgent[agentIndex];
        closestAgentIndex = agentIndex;
      }
    }

    for (int agentIndex = 0; agentIndex < agentsNumber; ++agentIndex)
    {
      double newDistanceToAgent = 
        getDistance(agentPositions[agentIndex], 
                    agentPositions[closestAgentIndex]);

      if (shortestDistanceToAgent[agentIndex] == NO_DISTANCE ||
          newDistanceToAgent < shortestDistanceToAgent[agentIndex])
      {
        shortestDistanceToAgent[agentIndex] = newDistanceToAgent;
      }
    }

    if (minimalSignalRadius < shortestDistance)
    {
      minimalSignalRadius = shortestDistance;
    }
    agentIsUsed[closestAgentIndex] = true;
  }

  return minimalSignalRadius;
}

void printOutput(double minimalSignalRadius)
{
  std::cout.precision(12);
  std::cout << std::fixed << minimalSignalRadius << std::endl;
}

int main()
{
  std::vector<Point> agentPositions;
  readInput(&agentPositions);

  double minimalSignalRadius = findMinimalSignalRadius(agentPositions);

  printOutput(minimalSignalRadius);

  return 0;
}

