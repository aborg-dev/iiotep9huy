#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

void readInput(vector<int>* array, int& targetElement)
{
  freopen("input.txt", "r", stdin);
  int arraySize;
  cin >> arraySize >> targetElement;
  array->resize(arraySize);
  for (int i = 0; i < arraySize; ++i)
  {
    cin >> (*array)[i];
  }
}

int stdFindKthElement(vector<int> array, int targetElement)
{
  sort(array.begin(), array.end());
  return array[targetElement];
}

void print(const vector<int>& v)
{
  for(int i = 0; i < v.size(); i++)
  {
    cout << v[i] << " ";
  }
  cout << endl;
}

int randomizedFindKthElemnt(vector<int> array, int targetElement)
{
  int L = 0;
  int R = array.size() - 1;
  print(array);
  while (L < R)
  {
    cout << "L " << L << " R " << R << endl;
    //cout << array[L] << " " << array[L + 1] << endl;
    int randomElement = rand() % (R - L + 1) + L;
    int i = L;
    int j = R;
    int x = array[randomElement];
    cout << "Partition around " << randomElement << " = " << x << endl;
    while (i < j)
    {
      while (array[i] < x)
        i++;
      while (array[j] > x)
        j--;
      if (i < j)
      {
        swap(array[i], array[j]);
        i++;
        j--;
      }
    }
    cerr << "i " << i << " j " << j << endl;

    print(array);
    // L .. i
    // j .. R
    cerr << "target: " << targetElement << endl;
    if (i - L >= targetElement)
    {
      R = i;
    }
    else
    {
      //cerr << "Here" << endl;
      //cerr << "before: " << targetElement << endl;
      //cout << i - L + 1 << endl;
      targetElement -= i - L + 1;
      //cerr << "after: " << targetElement << endl;
      L = i + 1;
    }
  }
  cout << "L " << L << " R " << R << endl;
  //cout << array[L] << endl;
  return array[L];
}

int main()
{
  vector<int> array;
  int targetElement;

  readInput(&array, targetElement);

  for(int i = 0; i < array.size(); i++)
  {
    int value_first = stdFindKthElement(array, i);
    int value_second = randomizedFindKthElemnt(array, i);
    if (value_first != value_second)
    {
      cerr << i << endl;
      cerr << value_first << " " << value_second << endl;
      cerr << "FAIL!" << endl;
      return -1;
    }
  }

  return 0;
}
