#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;

void readSequence(vector<int>* sequence)
{
  size_t length; 
  std::cin >> length;
  sequence->resize(length);

  for (int i = 0; i < length; ++i)
  {
    std::cin >> (*sequence)[i];
  }
}

void readInput(vector<int>* sequence_first, vector<int>* sequence_second)
{
  readSequence(sequence_first);
  readSequence(sequence_second);
}

size_t findLongestCommonSubsequenceLength(const vector<int>& sequence_first, 
                                          const vector<int>& sequence_second)
{
  size_t length_first = sequence_first.size();
  size_t length_second = sequence_second.size();
  // shortcut LongestCommonSubsequence = LCS
  vector< vector<size_t> > 
    LCSlengthEndingAt(length_first + 1, vector<size_t>(length_second + 1, 0));

  for (int index_first = 1; index_first <= length_first; ++index_first)
  {
    for (int index_second = 1; index_second <= length_second; ++index_second)
    {
      LCSlengthEndingAt[index_first][index_second] =
        std::max(LCSlengthEndingAt[index_first - 1][index_second], 
                 LCSlengthEndingAt[index_first][index_second - 1]);

      if (sequence_first[index_first - 1] == sequence_second[index_second - 1])
      {
        LCSlengthEndingAt[index_first][index_second] 
          = LCSlengthEndingAt[index_first - 1][index_second - 1] + 1;
      }
    }
  }

  return LCSlengthEndingAt[length_first][length_second];
}

void printOutput(size_t subsequence_length)
{
  std::cout << subsequence_length << std::endl;
}

int main()
{
  vector<int> sequence_first, sequence_second;
  readInput(&sequence_first, &sequence_second);
  size_t subsequence_length = findLongestCommonSubsequenceLength(sequence_first, sequence_second);
  printOutput(subsequence_length);
  return 0;
}
