#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;

struct Cube
{
  int dimension[3];
  explicit Cube(int dimension_[3])
  {
    std::copy(dimension_, dimension_ + 3, dimension);
  }
  bool contains(const Cube& cube) const
  {
    for (int i = 0; i < 3; ++i)
    {
      if (dimension[i] <= cube.dimension[i])
        return false;
    }
    return true;
  }
};

void readInput(vector<Cube>* array)
{
  int arraySize = 0;
  std::cin >> arraySize;
  for (int i = 0; i < arraySize; ++i)
  {
    int dimension[3];

    for (int j = 0; j < 3; ++j)
    {
      std::cin >> dimension[j];
    }

    (*array).push_back(Cube(dimension));
  }
}

bool canContinue(const Cube& first, const Cube& second)
{
  return second.contains(first);
}

int findLongestNestedSubsequenceLength(vector<Cube> array)
{
  vector<int> longestSubsequenceEndingAt(array.size(), 1);

  for (int currentEnd = 0; currentEnd < array.size(); ++currentEnd)
  {
    for (int previousEnd = 0; previousEnd < currentEnd; ++previousEnd)
    {
      if (canContinue(array[previousEnd], array[currentEnd]))
      {
        if (longestSubsequenceEndingAt[currentEnd] 
            < longestSubsequenceEndingAt[previousEnd] + 1)
        {
          longestSubsequenceEndingAt[currentEnd] = 
            longestSubsequenceEndingAt[previousEnd] + 1;
        }
      }
    }
  }
  int longestSubsequenceLength 
    = *std::max_element(longestSubsequenceEndingAt.begin(), longestSubsequenceEndingAt.end());

  return longestSubsequenceLength;
}

void printOutput(int subsequnceLenght)
{
  std::cout << subsequnceLenght << std::endl;
}

int main()
{
  vector<Cube> array;
  readInput(&array);
  int subsequenceLength = findLongestNestedSubsequenceLength(array);
  printOutput(subsequenceLength);
  return 0;
}
