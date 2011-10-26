#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>
#include <cassert>
#include <math.h>

using namespace std;

typedef long long i64;

const int MAXX = 1000000000;
const int MAXY = 1000000000;

struct TPoint
{
  int x, y;
  TPoint(){}
  TPoint(int x, int y): x(x), y(y) {}
};

inline double dist(TPoint &p1, TPoint &p2)
{
  return sqrt((i64)(p1.x-p2.x)*(p1.x-p2.x) + (i64)(p1.y-p2.y)*(p1.y-p2.y) + 0.0);
}

inline TPoint randPoint(int maxx = MAXX, int maxy = MAXY)
{
  return TPoint(rand()%maxx, rand()%maxy);
}

vector<TPoint> P;
vector<char> visited;
int n, N;

double calc(int start)
{
  int current = start, next;
  int C = P.size();
  visited.assign(C, 0);
  visited[current] = 1;

  double tourLength = 0.0;
  double nextDist, tempDist;
  for(int k=0; k<C-1; k++)
  {
    next = -1;
    nextDist = 1e15;
    for(int i=0; i<C; i++)
    {
      if (!visited[i])
      {
        tempDist = dist(P[current], P[i]);
        if (tempDist < nextDist)
        {
          nextDist = tempDist;
          next = i;
        }
      }
    }
    tourLength += nextDist;
    current = next;
    visited[current] = 1;
  }
  tourLength += dist(P[current], P[start]);
  return tourLength;
}


class AntiTravelingSalesperson {
public:
	vector<int> placeLocations(int N, vector<int> X, vector<int> Y) {
    srand(time(NULL));
    n = X.size();
    ::N = N;

    for(int i=0; i<n; i++)
      P.push_back(TPoint(X[i], Y[i]));

    for(int i=0; i<N; i++)
      P.push_back(randPoint());

    vector<int> answer;
    for(int i=0; i<N; i++)
    {
      answer.push_back(P[n+i].x);
      answer.push_back(P[n+i].y);
    }
    //fprintf(stderr, "%.9lf\n", res);
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
