#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>
#include <cassert>
#include <math.h>
#include <list>
#include <algorithm>
#include <memory.h>

#define index my_index

using namespace std;

typedef long long i64;

const int MAXX = 1000000000;
const int MAXY = 1000000000;

const double checkpoints[] = 
{
  0.81600, 0.87300, 0.83500, 0.80300, 0.81400, 0.86700, 0.78300, 0.83100, 0.82100, 0.84500, 
  0.87300, 0.87000, 0.86100, 0.80300, 0.78700, 0.87700, 0.83100, 0.79200, 0.84500, 0.78600,
};

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

inline i64 dist1(TPoint &p1, TPoint &p2)
{
  return ((i64)(p1.x-p2.x)*(p1.x-p2.x) + (i64)(p1.y-p2.y)*(p1.y-p2.y) + 0.0);
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

int n, N;

double calc_stupid(int start)
{
  int current = start, next;
  int C = P.size();

  vector<char> visited;
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
double calc_stl(int start)
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

  /*for(list<int>::iterator cur = V.begin(); (cur != V.end()); ++cur)
    cerr << *cur <<  " ";
  cerr << endl;*/

  list<int>::iterator cur, next;
  for(cur = V.begin(); (cur != V.end()) && !(*cur == current); ++cur);
  //V.erase(cur); // To Change

  double tourLength = 0.0;
  double nextDist, tempDist;
  for(int k=0; k<C-1; k++)
  {
    next = V.begin();
    nextDist = 1e15;
    //cerr << *cur << " ";
    for(list<int>::iterator it = cur;; --it)
    {
      if (it == cur) 
      {
        if (it == V.begin())
          break;
        continue;
      }

      tempDist = dist(P[current], P[*it]);
      if (nextDist < fabs(P[current].x - P[*it].x))
      {
        cnt++;
        break;
      }
      if (tempDist - nextDist < -1e-7 || (fabs(tempDist - nextDist) < 1e-7 && *next > *it))
      {
        nextDist = tempDist;
        next = it;
      }
      if (it == V.begin())
        break;
    }
    for(list<int>::iterator it = cur; it != V.end(); ++it)
    {
      if (it == cur) continue;
      tempDist = dist(P[current], P[*it]);
      if (nextDist < fabs(P[*it].x - P[current].x))
      {
        cnt++;
        break;
      }
      if (tempDist - nextDist < -1e-7 || (fabs(tempDist - nextDist) < 1e-7 && *next > *it))
      {
        nextDist = tempDist;
        next = it;
      }
    }
    V.erase(cur);
    tourLength += nextDist;
    cur = next;
    current = *cur;
  }
  tourLength += dist(P[*cur], P[start]);
  return tourLength;
}

int nx[15000], pr[15000], index[15000];
bool visited[15000];
double calc(int start)
{
  int current = start;
  int C = P.size();
  
  memset(visited, 0, sizeof(bool)*(C+1));
  visited[current] = 1;

  for(int i=1; i<=C; i++)
    index[i] = i-1;
  sort(index + 1, index + C + 1, compX);

  for(int i=1; i<=C; i++)
  {
    //cerr << index[i] << " ";
    pr[i] = i-1;
    nx[i] = i+1;
  }
  pr[0] = nx[C] = 0;

  int cur, next;
  for(cur = 1; (cur) && !(index[cur] == current); cur = nx[cur]);

  double tourLength = 0.0;
  double nextDist, tempDist;
  for(int k=0; k<C-1; k++)
  {
    next = 1;
    nextDist = 1e15;
    //cerr << current << " ";
    for(int it = pr[cur] ; it ; it = pr[it])
    {

      tempDist = dist(P[current], P[index[it]]);
      if (nextDist < fabs(P[current].x - P[index[it]].x))
        break;

      if (tempDist - nextDist < -1e-7 || (fabs(tempDist - nextDist) < 1e-7 && index[next] > index[it]))
      {
        nextDist = tempDist;
        next = it;
      }

    }
    for(int it = nx[cur]; it; it = nx[it])
    {
      tempDist = dist(P[current], P[index[it]]);
      if (nextDist < fabs(P[index[it]].x - P[current].x))
        break;
        
      if (tempDist - nextDist < -1e-7 || (fabs(tempDist - nextDist) < 1e-7 && index[next] > index[it]))
      {
        nextDist = tempDist;
        next = it;
      }
    }
    pr[nx[cur]] = pr[cur];
    nx[pr[cur]] = nx[cur];
    tourLength += nextDist;
    cur = next;
    current = index[cur];
  }
  tourLength += dist(P[index[cur]], P[start]);
  return tourLength;
}

void isotropyGen()
{
  for(int i=P.size(); i<n+N; i++)
    P.push_back(randPoint(1));
}

void webGen(double coef)
{
  //cerr << coef << endl;
  P.resize(n);
  int sqrtN = sqrt(max(N*coef + 0.0, 2.0));
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


void rombGen(double coef, double wide = 2.0)
{
  P.resize(n);
  int sqrtN = sqrt(N*coef + 0.0);
  int k = (1000000000 / sqrtN) / wide;
  for(int i=0; i<sqrtN; i++)
  {
    for(int j=0; j<sqrtN; j++)
    {
      int x, y;
      x = j*k*wide + (i%2)*k*wide / 2.0;
      y = i*k*wide;
      P.push_back(TPoint(x, y));
    }
  }
  isotropyGen();
}

vector<TPoint> best_distribution;
double best_answer = 0.0;
inline void relax_solution(double coef = 0.0)
{
  double current_answer = calc(0);
  if (current_answer > best_answer)
  {
    cerr << coef << endl;
    cerr << current_answer / 1000000000 << endl;
    best_distribution = P;
    best_answer = current_answer;
  }
}

void coefCheck()
{
  for(int i=0; i<20; i++)
  {
    webGen(checkpoints[i]);
    relax_solution();
  }
}

void bruteforce(int type = 0)
{
  if (type == 0)
  for(double i = 0.01; i < 0.999; i+=0.01)
  {
    cerr << i << endl;
    webGen(i);
    relax_solution();
  }
  if (type == 1)
  for(double wide = 1.0; wide <= 1.3; wide += 0.001)
  {
    //cerr << wide << endl;
    for(double i = 0.1; i < 0.999; i+=0.001)
    {
      //cerr << i << endl;
      for(int k=0; k<10; k++)
      {
        rombGen(i, wide);
        relax_solution(wide);
      }
    }
  }
  /*FILE *fout = fopen("coef.txt", "a+");
  fprintf(fout, "%.5lf, ", bestp);
  fclose(fout);*/
}

void generate_points()
{
  //srand(time(NULL));
  bruteforce(1);
  
  /*
  if (n+N < 3000)
    bruteforce();
  else
    coefCheck();
  */

}

void test_greedy()
{
  double t1 = clock();
  double ans1 = calc(0);
  double t2 = clock();
  //cerr << endl;
  double ans2 = calc_stl(0);
  double t3 = clock();
  //cerr << endl;
  fprintf(stderr, "Main: %lf\n", (t2 - t1) / 1000.0);
  fprintf(stderr, "Test: %lf\n", (t3 - t2) / 1000.0);
  assert(fabs(ans1 - ans2) < 1e9);
}

class AntiTravelingSalesperson 
{
public:
	vector<int> placeLocations(int N, vector<int> X, vector<int> Y) 
  {
    n = X.size();
    ::N = N;

    for(int i=0; i<n; i++)
      P.push_back(TPoint(X[i], Y[i]));

    generate_points();
    P = best_distribution;

    vector<int> answer;
    for(int i=0; i<N; i++)
    {
      answer.push_back(P[n+i].x);
      answer.push_back(P[n+i].y);
    }

    fprintf(stderr, "Execution time: %lf\n", clock() / 1000.0);

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

