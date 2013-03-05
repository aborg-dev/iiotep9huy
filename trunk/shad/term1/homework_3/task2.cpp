#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <functional>
#include <memory.h>

#define DB(x) std::cerr << #x << ": " << x << std::endl;

using std::vector;

const size_t MAXSIZE = 200009;

struct Coin
{
  int position, expireTime;
};

void readInput(vector<Coin>* coins)
{
  int coinNumber;
  std::cin >> coinNumber;
  coins->resize(coinNumber);

  for (int i = 0; i < coinNumber; ++i)
  {
    std::cin >> (*coins)[i].position >> (*coins)[i].expireTime;
  }
}

int calculateRequiredTime(const vector<Coin>& coins)
{
  size_t coinNumber = coins.size();
  vector< vector< pair<int, int> > > minRequiredTime(coinNumber, vector< pair<int, int> > (coinNumber, make_pair(-1, -1)));

  for (int coinIndex = 0; coinIndex < coinNumber; ++coinIndex)
    minRequiredTime[coinIndex][coinIndex] = 0;

  for (int lenght = 2; lenght < coinNumber; ++lenght)
  { 
    for (int coinIndex = 0; coinIndex < coinNumber - lenght + 1; ++coinIndex)
    {
      pair<int, int> currentState = &minRequiredTime[coinIndex][coinIndex + lenght - 1];

      if (minRequiredTime[coinIndex][coinIndex + lenght - 2].first != -1)
      {
        currentState.first = min(currentState.first, );
      }

      if (currentState.first > coins[coinIndex].expireTime)
        currentState.first = -1;

      if (currentState.second > coins[coinIndex + length - 1].expireTime)
        currentState.second = -1;
    }
  }
  int bestRequiredTime = 1000000000;
  if (minRequiredTime[0][coinNumber - 1].first != -1)
    bestRequiredTime = min(bestRequiredTime, minRequiredTime[0][coinNumber - 1].first);

  if (minRequiredTime[0][coinNumber - 1].second != -1)
    bestRequiredTime = min(bestRequiredTime, minRequiredTime[0][coinNumber - 1].second);

  return bestRequiredTime;
}

void printOutput(int requiredTime)
{
  std::cout << requiredTime << std::endl;
}

/*
void gen(int n, int m)
{
  freopen("input.txt", "w", stdout);
  printf("%d %d\n", n, m);
  for (int i = 0; i < n; ++i)
  {
    vector<int> v(m);
    for (int j = 0; j < m; ++j)
      v[j] = rand() % 100;
    std::sort(v.begin(), v.end());
    for (int j = 0; j < m; ++j)
      std::cout << v[j] << " ";
    std::cout << std::endl;
  }
  exit(0);
}
*/

int main()
{
  // gen(100, 100);
  freopen("input.txt", "r", stdin); // REMOVE
  vector<Coin> coins;
  readInput(&coins);

  int requiredTime = calculateRequiredTime(coins);

  printOutput(requiredTime);
  return 0;
}
