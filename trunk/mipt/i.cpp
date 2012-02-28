#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <string.h>
#include <cassert>
#include <math.h>
#include <vector>
#include <time.h>
#include <set>
#include <queue>
#include <map>
#include <complex.h>
#define REP(i,n) for(int i=0, _n=(n); i<_n; i++)
#define REPD(i,n) for(int i=n-1; i>=0; i--)
#define FOR(i,a,b) for(int i=a,_b=(b); i<=_b; i++)
#define FORD(i,a,b) for(int i=a,_b=(b); i>=_b; i--)
#define FILL(x, v) memset(&x,v,sizeof(x))
#define DB(vari) cerr<<#vari<<" = "<<vari<<endl;
#define x first
#define y second
#define mp make_pair
#define pb push_back

using namespace std;

typedef pair<int, int> pii;
typedef long long LL;
typedef complex<double> base;

const int MAXW = 809;
const int MAXH = 809;
const double PI = acos(-1.0);

int W, H, R, C;

vector<int> field;
vector<int> pattern;

void fft(vector<base>& a, bool invert)
{
  size_t n = a.size();
  if (n == 1)
    return;
  vector<base> odd(n / 2), even(n / 2);
  for(int i = 0; i < n / 2; i++)
  {
    even[i] = a[i * 2];
    odd[i]  = a[i * 2 + 1];
  }
  fft(even, invert);
  fft(odd, invert);
  double Ang = 2 * PI / n * (invert ? -1 : 1);
  base w(1), wn(cos(Ang), sin(Ang));
  for(int i = 0; i < n / 2; i++)
  {
    a[i] = even[i] + w * odd[i];
    a[i + (n / 2)] = even[i] - w * odd[i];
    if (invert)
    {
      a[i] /= 2;
      a[i + (n / 2)] /= 2;
    }
    w *= wn;
  }
}

void multiply(vector<int>& a, vector<int>& b, vector<int>& c)
{
  int n = 1;
  while (n < a.size())
    n <<= 1;
  n <<= 1;
  vector<base> fa(a.begin(), a.end()), fb(b.begin(), b.end());
  fa.resize(n);
  fb.resize(n);
  fft(fa, 0);
  fft(fb, 0);
  for(int i = 0; i < n; i++)
    fa[i] *= fb[i];
  fft(fa, 1);
  c.resize(n);
  for(int i = 0; i < n; i++)
    c[i] = int(fa[i].real() + 0.5);
}

void print(vector<int> v)
{
  for(int i = 0; i < v.size(); i++)
    printf("%d ", v[i]);
  printf("\n");
}

int main()
{
	freopen("i.in","r", stdin); freopen("i.out","w", stdout);
  scanf("%d%d\n", &R, &C);
  field.resize(R * C);
  REP(i, R)
  {
    REP(j, C)
    {
      char ch;
      scanf("%c", &ch);
      field[i * C + j] = ch - '0';
    }
    scanf("\n");
  }
  scanf("%d%d\n", &H, &W);
  pattern.assign(W * C, 0);
  REP(i, H)
  {
    REP(j, W)
    {
      char ch;
      scanf("%c", &ch);
      pattern[i * C + j] = (ch == '#');
    }
    scanf("\n");
  }
  vector<int> result;
  multiply(field, pattern, result);
  print(field);
  print(pattern);
  print(result);
  int best_ans = -100000;
  int n = result.size();
  for(int i = 0; i < n / 2; i++)
    best_ans = max(best_ans, result[i] + result[i + n/2]);
  printf("%d\n", best_ans);
	return 0;
}
