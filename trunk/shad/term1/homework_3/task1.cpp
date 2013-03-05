#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <functional>
#include <memory.h>
#include <string>
#include <math.h>

#define DB(x) std::cerr << #x << ": " << x << std::endl;

using std::vector;
using std::string;
using std::cin;

const size_t MAXSIZE = 524288;
const size_t MAXLEN = 200009;

char buffer[MAXLEN];

void readInput(string* sequence_first, string* sequence_second)
{
  gets(buffer);
  *sequence_first = string(buffer);

  gets(buffer);
  *sequence_second = string(buffer);
}

namespace fft
{

const double pi = acos(-1.0);

struct complex
{
  complex(double real = 0.0, double imag = 0.0) : real(real), imag(imag) {}
  double real, imag;
};

complex operator+ (const complex& c_first, const complex& c_second)
{
  return complex(c_first.real + c_second.real, c_first.imag + c_second.imag);
}

complex operator- (const complex& c_first, const complex& c_second)
{
  return complex(c_first.real - c_second.real, c_first.imag - c_second.imag);
}

complex operator* (const complex& c_first, const complex& c_second)
{
  return complex(c_first.real * c_second.real - c_first.imag * c_second.imag, 
                  c_first.real * c_second.imag + c_first.imag * c_second.real);
}

int rev[MAXSIZE];

void buildRev(int length)
{
  rev[0] = 0;
  for (int i = 1; i < length; i <<= 1)
  {
    for (int j = 0; j < i; ++j)
    {
      rev[j] = rev[j] * 2;
      rev[j + i] = rev[j] + 1;
    }
  }
}

void fft(complex* array, int length, bool invert)
{
  buildRev(length);
  for (int i = 0; i < length; ++i)
  {
    if (i > rev[i])
    {
      std::swap(array[i], array[rev[i]]);
    }
  }

  for (int bSize = 2; bSize <= length; bSize <<= 1)
  {
    double arg = (2 * pi / bSize) * (invert ? -1 : 1);
    complex wn(cos(arg), sin(arg));
    for (int start = 0; start < length; start += bSize)
    {
      complex w(1.0, 0);
      for (int pos = 0; pos < bSize / 2; ++pos)
      {
        complex u = array[start + pos];
        complex v = array[start + pos + bSize / 2] * w;
        array[start + pos] = u + v;
        array[start + pos + bSize / 2] = u - v;
        w = w * wn;
      }
    }
  }
  if (invert)
  {
    for (int i = 0; i < length; ++i)
    {
      array[i] = array[i] * (1.0 / length);
    }
  }
}

vector<int> mult(const vector<int>& v_first, const vector<int>& v_second)
{
  size_t length = 1;
  while (length < std::max(v_first.size(), v_second.size()))
    length <<= 1;
  length <<= 1;

  complex* a = new complex[length];
  complex* b = new complex[length];
  memset(a, 0, sizeof(complex) * length);
  memset(b, 0, sizeof(complex) * length);
  std::copy(v_first.begin(), v_first.end(), a);
  std::copy(v_second.begin(), v_second.end(), b);

  fft(a, length, 0);
  fft(b, length, 0);
  for (int i = 0; i < length; ++i)
  {
    a[i] = a[i] * b[i];
  }

  fft(a, length, 1);

  std::vector<int> result(length);
  for (int i = 0; i < length; ++i)
  {
    result[i] = static_cast<int> (a[i].real + 0.5);
  }

  delete a;
  delete b;
  return result;
}

} // namespace fft

std::ostream& operator<< (std::ostream& stream, const std::vector<int>& vector)
{
  for (int i = 0; i < vector.size(); ++i)
  {
    stream << vector[i];
  }
  stream << std::endl;
  return stream;
}

const char ALPHA[] = {"AGTC"};

int findBestShift(const string& sequence_first, const string& sequence_second)
{
  int length_first = sequence_first.length();
  int length_second = sequence_second.length();
  int rLen = length_first - length_second + 1;
  vector<int> scalar_product(rLen);
  vector<int> poly_first(length_first), poly_second(length_second);
  for (int letterNumber = 0; letterNumber < 4; ++letterNumber)
  {
    for (int i = 0; i < length_first; ++i)
    {
      poly_first[i] = (sequence_first[i] == ALPHA[letterNumber]);
    }
    for (int i = 0; i < length_second; ++i)
    {
      poly_second[length_second - i - 1] = (sequence_second[i] == ALPHA[letterNumber]);
    }
    vector<int> product = fft::mult(poly_first, poly_second);
    for (int i = length_first - 1; i >= length_second - 1; --i)
    {
      scalar_product[i - length_second + 1] += product[i];
      if (i < length_first)
      {
        scalar_product[i - length_second + 1] += product[i + length_first];
      }
    }
  }
  int bestShift = 0;
  for (int i = 0; i < scalar_product.size(); ++i)
  {
    if (scalar_product[i] > scalar_product[bestShift])
      bestShift = i;
  }
  return bestShift + 1;
}

void printOutput(int shift)
{
  std::cout << shift << std::endl;
}

void gen(int n, int m)
{
  freopen("input.txt", "w", stdout);
  //printf("%d %d\n", n, m);
  for (int i = 0; i < n; ++i)
  {
    printf("%c", ALPHA[rand()%4]);
  }
  printf("\n");
  for (int i = 0; i < m; ++i)
  {
    printf("%c", ALPHA[rand()%4]);
  }
  exit(0);
}

int main()
{
  //gen(200000, 100000);
  int t_first = clock();

  freopen("input.txt", "r", stdin); // REMOVE

  string sequence_first, sequence_second;
  readInput(&sequence_first, &sequence_second);

  int bestShift = findBestShift(sequence_first, sequence_second);

  printOutput(bestShift);

  int t_second = clock();
  std::cerr  << double(t_second - t_first) / CLOCKS_PER_SEC << std::endl;
  return 0;
}
