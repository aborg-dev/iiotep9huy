#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;

void readInput(vector<int>* array)
{
  int arraySize = 0;
  std::cin >> arraySize;
  array->assign(arraySize, 0);
  for (int i = 0; i < arraySize; ++i)
  {
    std::cin >> (*array)[i];
  }
}

bool canContinue(bool type, int first, int second)
{
  if (type == 0)
    return first > second;
  else
    return first < second;
}

vector<int> restoreLongestSubsequence(const vector< vector<int> >& longestSubsequencePreviousPick,
                                      int longestSubsequenceEnd, int longestSubsequenceType)
{
  vector<int> subsequence;

  while (longestSubsequenceEnd != -1)
  {
    subsequence.push_back(longestSubsequenceEnd);
    longestSubsequenceEnd = 
      longestSubsequencePreviousPick[longestSubsequenceEnd][longestSubsequenceType];
    longestSubsequenceType = 1 - longestSubsequenceType;
  }

  return subsequence;
}

void findLongestSubsequenceEnd(int arraySize, 
                              const vector< vector<int> >& longestSubsequenceEndingAt,
                              int& longestSubsequenceEnd, int& longestSubsequenceType)
{
  longestSubsequenceEnd = 0;
  longestSubsequenceType = 0;
  int longestSubsequenceLength = 1;
  for (int end = arraySize - 1; end >= 0; --end)
  {
    for (int type = 0; type < 2; ++type)
    {
      if (longestSubsequenceEndingAt[end][type] > longestSubsequenceLength)
      {
        longestSubsequenceLength = longestSubsequenceEndingAt[end][type];
        longestSubsequenceEnd = end;
        longestSubsequenceType = type;
      }
    }
  }
}

vector<int> findLongestAlternatingSubsequence(vector<int> array)
{
  reverse(array.begin(), array.end());
  vector< vector<int> > longestSubsequenceEndingAt(array.size(), vector<int>(2, 1));
  vector< vector<int> > longestSubsequencePreviousPick(array.size(), vector<int>(2, -1));

  for (int currentEnd = 0; currentEnd < array.size(); ++currentEnd)
  {
    for (int type = 0; type < 2; ++type)
    {
      for (int previousEnd = currentEnd - 1; previousEnd >= 0; --previousEnd)
      {
        if (canContinue(type, array[previousEnd], array[currentEnd]))
        {
          if (longestSubsequenceEndingAt[currentEnd][type] 
              < longestSubsequenceEndingAt[previousEnd][1 - type] + 1)
          {
            longestSubsequenceEndingAt[currentEnd][type] = 
              longestSubsequenceEndingAt[previousEnd][1 - type] + 1;
            longestSubsequencePreviousPick[currentEnd][type] = previousEnd;
          }
        }
      }
    }
  }
  int longestSubsequenceEnd = 0;
  int longestSubsequenceType = 0;
  findLongestSubsequenceEnd(array.size(), longestSubsequenceEndingAt, 
                            longestSubsequenceEnd, longestSubsequenceType);

  vector<int> subsequence = 
    restoreLongestSubsequence(longestSubsequencePreviousPick, 
                              longestSubsequenceEnd, longestSubsequenceType);

  for (int i = 0; i < subsequence.size(); ++i)
    subsequence[i] = array[subsequence[i]];

  return subsequence;
}

void printOutput(const vector<int>& subsequnce)
{
  for (int i = 0; i < subsequnce.size(); ++i)
  {
    std::cout << subsequnce[i] << " ";
  }
  std::cout << std::endl;
}

int main()
{
  vector<int> array;
  readInput(&array);
  vector<int> subsequence = findLongestAlternatingSubsequence(array);
  printOutput(subsequence);
  return 0;
}
