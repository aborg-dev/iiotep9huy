#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

vector<double> bestScore;
vector<int> bestN;
vector<double> curScore;
vector<int> curN;

void parse(FILE *f, vector<double> &score, vector<int> &N)
{
  char ch;
  double curScore;
  int n;
  while (!feof(f))
  {
    fscanf(f, "%c", &ch);
    if (feof(f)) return;
    if (ch == 'N')
    {
      fscanf(f, " = %d", &n);
      N.push_back(n);
    }
    if (ch == 'S')
    {
      fscanf(f, "core = %lf", &curScore);
      score.push_back(curScore);
    }
  }
}

int main()
{
  FILE *best = fopen("best.txt", "r");
  FILE *cur = fopen("res.txt", "r");
  parse(best, bestScore, bestN);
  parse(cur, curScore, curN);
  /*if (bestScore.size() != curScore.size())
  {
    cout << "bad input format" << endl;
    return 0;
  }*/
  int bestBetter = 0;
  for(int i=0; i<min(bestScore.size(), curScore.size()); i++)
  {
    printf("%5d: %8.3lf  %8.3lf", bestN[i], bestScore[i], curScore[i]);
    if (bestScore[i] > curScore[i])
      printf("  best"), bestBetter++;
    else
      printf("  cur");
    printf("\n");
  }
  printf("%d %d\n", bestBetter, bestScore.size() - bestBetter);
  return 0;
}
