#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>
#include <cassert>
#include <math.h>
#include <list>
#include <algorithm>

using namespace std;

typedef long long i64;

const int MAXX = 1000000000;
const int MAXY = 1000000000;

struct TPoint
{
  int x, y;
  TPoint(){}
  TPoint(int x, int y): x(x), y(y) {}
  bool operator==(const TPoint &v)
  {
    return (x == v.x) && (y == v.y);
  }
};

inline double dist(TPoint &p1, TPoint &p2)
{
  return sqrt((i64)(p1.x-p2.x)*(p1.x-p2.x) + (i64)(p1.y-p2.y)*(p1.y-p2.y) + 0.0);
}

inline TPoint randPoint(int type, int minx = 0, int miny = 0, int maxx = MAXX, int maxy = MAXY)
{
  switch (type)
  {
    case 1:
    {
      return TPoint(rand()%maxx, rand()%maxy);
    }
    case 2:
    {
      int choice = rand()&1;
      if (choice == 0)
        return TPoint(rand()%maxx, miny);
      else
        return TPoint(rand()%maxx, maxy);
    }
  }
  return TPoint(rand()%maxx, rand()%maxy);
}

vector<TPoint> P;

bool compX(const int a, const int b)
{
  if (P[a].x != P[b].x)
    return P[a].x < P[b].x;
  return P[a].y < P[b].y;
}

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

int cnt;
double fastCalc(int start)
{
  int current = start;
  int C = P.size();
  
  vector<char> visited;
  visited.assign(C, 0);
  visited[current] = 1;

  list<int> V;
  for(int i=0; i<C; i++)
    V.push_back(i);

  V.sort(compX);

  list<int>::iterator cur, next;
  for(cur = V.begin(); (cur != V.end()) && !(*cur == current); ++cur);
  V.erase(cur); // To Change

  double tourLength = 0.0;
  double nextDist, tempDist;
  for(int k=0; k<C-1; k++)
  {
    next = V.begin();
    nextDist = 1e15;
    for(list<int>::iterator it = V.begin(); it != V.end(); ++it)
    {
      //cerr << nextDist << endl;
      //cerr << (P[current].x - P[*it].x) << endl;
      //cerr << (nextDist - (P[current].x - P[*it].x)) << endl;
      tempDist = dist(P[current], P[*it]);
      if (nextDist < fabs(P[current].x - P[*it].x + 0.0))
      {
        cnt++;
        break;
      }
      if (tempDist < nextDist)
      {
        nextDist = tempDist;
        next = it;
      }
    }
    tourLength += nextDist;
    cur = next;
    current = *cur;
    V.erase(cur); // To Change
  }
  tourLength += dist(P[*cur], P[start]);
  return tourLength;
}

void isotropyGen()
{
  for(int i=P.size(); i<n+N; i++)
    P.push_back(randPoint(1));
}

void webGen()
{
  double coef = 11.0/(log(N)/log(2.0));
  //cerr << coef << endl;
  int sqrtN = sqrt(N/1.2 + 0.0);
  int d = 1000000000 / sqrtN;
  for(int i=0; i<sqrtN; i++)
  {
    for(int j=0; j<sqrtN; j++)
    {
      P.push_back(TPoint(i*d, j*d));
    }
  }

  isotropyGen();
}


class AntiTravelingSalesperson {
public:
	vector<int> placeLocations(int N, vector<int> X, vector<int> Y) 
  {
    //srand(time(NULL));
    n = X.size();
    ::N = N;

    for(int i=0; i<n; i++)
      P.push_back(TPoint(X[i], Y[i]));

    //isotropyGen();
    webGen();

    vector<int> answer;
    for(int i=0; i<N; i++)
    {
      answer.push_back(P[n+i].x);
      answer.push_back(P[n+i].y);
    }

    double t1 = clock();
    double ans1 = calc(0);
    double t2 = clock();
    double ans2 = fastCalc(0);
    double t3 = clock();
    cerr << cnt << endl;
    fprintf(stderr, "%lf\n", (t2 - t1) / 1000.0);
    fprintf(stderr, "%lf\n", (t3 - t2) / 1000.0);
    assert(ans1 == ans2);
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
