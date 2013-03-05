#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <stdexcept>
#include <functional>
#include <memory.h>
#include <memory>
#include <iomanip>
#include <math.h>
#include <queue>

#define DB(x) std::cerr << #x << ": " << x << std::endl;

const double INF = 1e15;
const double eps = 1e-11;

// cpplint prevents me to name xCord as x and yCord as y. I don't like it!
struct Point
{
  Point() 
  {
    xCord = yCord = 0;
  }

  Point(int xCord, int yCord): xCord(xCord), yCord(yCord) {}

  int xCord, yCord;
};

std::istream& operator>> (std::istream& stream, Point& point)
{
  stream >> point.xCord >> point.yCord;
  return stream;
}

std::ostream& operator<< (std::ostream& stream, const Point& point)
{
  stream << point.xCord << " " << point.yCord << std::endl;
  return stream;
}

int randomCord(int upperBound)
{
  int sign = rand()&1 ? 1 : -1;
  return sign * (rand() % upperBound);
}

Point randomPoint(int upperBound)
{
  return Point(randomCord(upperBound), randomCord(upperBound));
}

double sqr(double value)
{
  return value * value;
}

double getDistance(const Point& p_first, const Point& p_second)
{
  return sqrt(sqr(p_first.xCord - p_second.xCord) 
            + sqr(p_first.yCord - p_second.yCord)); 
}

void readInput(std::vector<Point>* agentPosition)
{
  int agentNumber;
  std::cin >> agentNumber;

  agentPosition->resize(agentNumber);

  for (int i = 0; i < agentNumber; ++i)
  {
    std::cin >> (*agentPosition)[i];
  }
}

bool checkRadiusSufficiency(const std::vector<Point>& agentPosition, double radius)
{
  int agentNumber = agentPosition.size();
  
  std::vector< std::vector<int> > adjacencyList(agentNumber, std::vector<int>());

  for (int firstAgent = 0; firstAgent < agentNumber; ++firstAgent)
  {
    for (int secondAgent = 0; secondAgent < agentNumber; ++secondAgent)
    {
      if (firstAgent != secondAgent)
      {
        if (getDistance(agentPosition[firstAgent], agentPosition[secondAgent]) <= radius + eps)
        {
          adjacencyList[firstAgent].push_back(secondAgent);
        }
      }
    }
  }

  std::queue<int> queue;
  queue.push(0);

  std::vector<char> vertexIsVisited(agentNumber, 0);
  vertexIsVisited[0] = true;

  while (!queue.empty())
  {
    int currentVertex = queue.front(); 
    queue.pop();
    for (const auto& nextVertex : adjacencyList[currentVertex])
    {
      if (!vertexIsVisited[nextVertex])
      {
        queue.push(nextVertex);
        vertexIsVisited[nextVertex] = true;
      }
    }
  }

  return std::all_of(vertexIsVisited.begin(), vertexIsVisited.end(), 
    [](char flag) 
    { 
      return flag; 
    }
  ); 
}

double findMinimalSignalRadiusTrivial(const std::vector<Point>& agentPosition)
{
  int agentNumber = agentPosition.size();

  double lowerBound = 0;
  double upperBound = INF;
  
  for (int iteration = 0; iteration < 200; ++iteration)
  {
    double middleRadius = (lowerBound + upperBound) / 2;
    if (checkRadiusSufficiency(agentPosition, middleRadius))
    {
      upperBound = middleRadius;
    }
    else
    {
      lowerBound = middleRadius;
    }
  }
  return lowerBound;
}

double findMinimalSignalRadius(const std::vector<Point>& agentPosition)
{
  int agentNumber = agentPosition.size();

  std::vector<double> smallestDistanceToVertex(agentNumber, INF);
  std::vector<char> vertexIsUsed(agentNumber, 0);

  smallestDistanceToVertex[0] = 0.0;

  double minimalSignalRadius = 0.0;

  for (int iteration = 0; iteration < agentNumber; ++iteration)
  {
    double smallestDistance = INF;
    int closestVertexIndex = -1;

    for (int vertexIndex = 0; vertexIndex < agentNumber; ++vertexIndex)
    {
      if (vertexIsUsed[vertexIndex])
      {
        continue;
      }
      if (smallestDistanceToVertex[vertexIndex] < smallestDistance)
      {
        smallestDistance = smallestDistanceToVertex[vertexIndex];
        closestVertexIndex = vertexIndex;
      }
    }

    for (int vertexIndex = 0; vertexIndex < agentNumber; ++vertexIndex)
    {
      smallestDistanceToVertex[vertexIndex] = std::min(smallestDistanceToVertex[vertexIndex], 
        getDistance(agentPosition[vertexIndex], agentPosition[closestVertexIndex]));
    }

    minimalSignalRadius = std::max(minimalSignalRadius, smallestDistance);
    vertexIsUsed[closestVertexIndex] = true;
  }

  return minimalSignalRadius;
}

void printOutput(double minimalSignalRadius)
{
  std::cout.precision(12);
  std::cout << std::fixed << minimalSignalRadius << std::endl;
}

void checkAnswerCorrectness(const std::vector<Point>& agentPosition, double minimalSignalRadius)
{
  if (!checkRadiusSufficiency(agentPosition, minimalSignalRadius))
  {
    std::logic_error logicError("Calculated radius is not sufficient");
    throw logicError;
  }

  if (checkRadiusSufficiency(agentPosition, minimalSignalRadius - 1.0))
  {
    std::logic_error logicError("Calculated radius is bigger than the sufficient one");
    throw logicError;
  }
}

void generateRandomTest(std::string testFileName)
{
  std::ofstream testFile(testFileName);

  srand(42);

  int agentNumber = 10;
  testFile << agentNumber << std::endl;

  for (int agentIndex = 0; agentIndex < agentNumber; ++agentIndex)
  {
    Point agentPosition = randomPoint(1000);
    testFile << agentPosition;
  }

  testFile.close();
}

void testRandom()
{
  generateRandomTest("test.txt");
  freopen("test.txt", "r", stdin);

  std::vector<Point> agentPosition;
  readInput(&agentPosition);

  double minimalSignalRadius = findMinimalSignalRadius(agentPosition);
  double minimalSignalRadiusTrivial = findMinimalSignalRadiusTrivial(agentPosition);

  std::cerr << "found minimalSignalRadius: " << minimalSignalRadius << std::endl;
  std::cerr << "found minimalSignalRadiusTrivial: " << minimalSignalRadiusTrivial << std::endl;

  try {
    checkAnswerCorrectness(agentPosition, minimalSignalRadius);
  } catch (const std::exception& e) {
    std::cerr << "Caught exception: \"" << e.what() << "\"\n";
  }
}

int main()
{
  // testRandom();

  std::vector<Point> agentPosition;
  readInput(&agentPosition);

  double minimalSignalRadius = findMinimalSignalRadius(agentPosition);

  printOutput(minimalSignalRadius);

  return 0;
}
