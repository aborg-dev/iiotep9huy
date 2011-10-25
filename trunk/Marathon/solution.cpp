#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>
#include <cassert>

using namespace std;

long long distance(int X1, int Y1, int X2, int Y2)
{
  
}

long long tourLen()
{
  currentLocation = initialLocation
  visited[currentLocation] = true
  tourLength = 0 
  while ( count(unvisited) > 0 )
  {
    nextLocation = unvisitedNearestTo(currentLocation) 
    tourLength += distance(currentLocation, nextLocation)
    currentLocation = nextLocation
    visited[currentLocation] = true
  }
  tourLength += distance(currentLocation, initialLocation)
}

class AntiTravelingSalesperson {
public:
	vector<int> placeLocations(int N, vector<int> X, vector<int> Y) {
    vector<int> answer;
    srand(time(NULL));
    for(int i=0; i<N; i++)
    {
      answer.push_back(abs(rand()%1000000000));
      answer.push_back(abs(rand()%1000000000));
    }
    return answer;
	}

};

int main()
{

  int N;
  scanf("%d", &N);

  int xLen;
  scanf("%d", &xLen);
  vector<int> X(xLen);
  for (int i=0; i < xLen; i++)
      scanf("%d", &X[i]);

  int yLen;
  scanf("%d", &yLen);
  vector<int> Y(yLen);
  for (int i=0; i < yLen; i++)
      scanf("%d", &Y[i]);

  AntiTravelingSalesperson solution;
  vector<int> ret = solution.placeLocations(N, X, Y);

  printf("%d\n", ret.size());
  for (int i=0; i < ret.size(); i++)
    printf("%d\n", ret[i]);

  fflush(stdout);

  return 0;
}
