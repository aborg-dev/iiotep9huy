#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <array>
#include <set>
#include <string>
#include <algorithm>
#include <math.h>
#include <stdexcept>

#define DB(x) std::cerr << #x << ": " << x << std::endl;

const int ALPHABET_SIZE = 256;

std::vector<int> buildSuffixArray(std::string string)
{
  string += '$';
  int length = string.length();
  std::vector<int> order(length);
  std::vector<int> nextOrder(length);
  std::vector<int> orderClass(length);
  std::vector<int> nextOrderClass(length);
  std::vector<int> counter(ALPHABET_SIZE, 0);

  for (int i = 0; i < length; ++i)
  {
    counter[string[i] + 1]++; 
  }

  std::partial_sum(counter.begin(), counter.end(), counter.begin());

  for (int i = 0; i < length; ++i)
  {
    order[counter[string[i]]++] = i;
  }

  int classCount = 1;
  orderClass[0] = 0;
  for (int i = 1; i < length; ++i)
  {
    if (string[order[i]] != string[order[i - 1]])
      classCount++;
    orderClass[order[i]] = classCount - 1;
  }

  counter.assign(length, 0);
  for (int iteration = 1; iteration < length; iteration *= 2)
  {
    for (int i = 0; i < length; ++i)
    {
      nextOrder[i] = order[i] - iteration;
      if (nextOrder[i] < 0)
        nextOrder[i] += length;
    }

    for (int i = 0; i < length; ++i)
    {
      counter[orderClass[i] + 1]++;
    }

    std::partial_sum(counter.begin(), counter.end(), counter.begin());

    for (int i = 0; i < length; ++i)
    {
      order[counter[orderClass[nextOrder[i]]]++] = nextOrder[i];
    }

    classCount = 1;
    nextOrderClass[order[0]] = 0;
    for (int i = 1; i < length; ++i)
    {
      if ((orderClass[order[i]] != orderClass[order[i - 1]]) || 
          (orderClass[order[i]] == orderClass[order[i - 1]] 
          && (orderClass[(order[i] + iteration) % length] != 
              orderClass[(order[i - 1] + iteration) % length])))
        classCount++;
      nextOrderClass[order[i]] = classCount - 1; 
    }

    orderClass = nextOrderClass;
    counter.assign(length, 0);
  }
  return std::vector<int>(order.begin() + 1, order.end());
}

std::vector<int> buildInversePermutation(const std::vector<int>& permutation)
{
  std::vector<int> inversePermutation(permutation.size());
  for (size_t i = 0; i < permutation.size(); ++i)
  {
    inversePermutation[permutation[i]] = i;
  }
  return inversePermutation;
}

// LCP stands for Longest Common Prefix
std::vector<int> buildLCPArray(const std::string& string, const std::vector<int>& suffixArray)
{
  int length = string.length();
  std::vector<int> lcpArray(length);
  std::vector<int> inverseSuffixArray = buildInversePermutation(suffixArray);

  int commonLength = 0;
  for (int suffixIndex = 0; suffixIndex < length; ++suffixIndex)
  {
    int position = inverseSuffixArray[suffixIndex];
    int suffixStart = suffixArray[position];
    int nextSuffixStart = suffixArray[(position + 1) % length];
    while ((suffixStart + commonLength < length) && (nextSuffixStart + commonLength < length))
    {
      if (string[suffixStart + commonLength] == string[nextSuffixStart + commonLength])
      {
        ++commonLength;
      }
      else
      {
        break;
      }
    }
    lcpArray[position] = commonLength;
    commonLength = std::max(0, commonLength - 1);
  }
  return lcpArray;
}

class SparseTableRMQ
{
public:
  SparseTableRMQ()
  {
  }

  void buildClosestLowerExponents(int length)
  {
    closestLowerExponent.resize(length + 1);
    int exponent = 0;
    int power = 1;
    for (int i = 0; i <= length; ++i)
    {
      while (power * 2 < i)
      {
        power *= 2;
        ++exponent;
      }
      closestLowerExponent[i] = exponent;
    }
  }

  void buildSparseTable(const std::vector<int>& array)
  {
    int length = array.size();
    int logLength = closestLowerExponent[length] + 1;
    sparseTable.assign(length, std::vector<int>(logLength, 0));
    for (int i = 0; i < length; ++i)
    {
      sparseTable[i][0] = array[i];
    }
    int power = 1;
    for (int exponent = 1; exponent < logLength; ++exponent)
    {
      for (int i = 0; i < length; ++i)
      {
        sparseTable[i][exponent] = sparseTable[i][exponent - 1];
        if (i + power < length)
        {
          sparseTable[i][exponent] = std::min(sparseTable[i][exponent], 
                                              sparseTable[i + power][exponent - 1]);
        }
      }
      power *= 2;
    }
  }

  void init(const std::vector<int>& array)
  {
    buildClosestLowerExponents(array.size());
    buildSparseTable(array);
  }

  int getMinOnRange(int leftBound, int rightBound) const
  {
    int distance = rightBound - leftBound;
    int exponent = closestLowerExponent[distance];
    int power = (1 << exponent);
    return std::min(sparseTable[leftBound][exponent], sparseTable[rightBound - power][exponent]);
  }

private:
  std::vector<size_t> closestLowerExponent; 
  std::vector<std::vector<int>> sparseTable;
};

std::vector<int> findMaxLengths(std::string string)
{
  std::vector<int> maxLengths;
  std::vector<int> suffixArray = buildSuffixArray(string);
  std::vector<int> inverseSuffixArray = buildInversePermutation(suffixArray);
  std::vector<int> lcpArray = buildLCPArray(string, suffixArray);
  SparseTableRMQ sparseTableRMQ;
  sparseTableRMQ.init(lcpArray);
  std::set<int> processedSuffixesPositions;

  for (int suffixLength = string.length(); suffixLength > 0; --suffixLength)
  {
    int position = inverseSuffixArray[string.length() - suffixLength];
    int maxLength = 0;
    auto smallerSuffixPositionIterator = processedSuffixesPositions.lower_bound(position);
    if (smallerSuffixPositionIterator != processedSuffixesPositions.begin())
    {
      --smallerSuffixPositionIterator;
      int commonLength = sparseTableRMQ.getMinOnRange(*smallerSuffixPositionIterator, position);
      maxLength = std::max(maxLength, commonLength);
    }
    auto greaterSuffixPositionIterator = processedSuffixesPositions.upper_bound(position);
    if (greaterSuffixPositionIterator != processedSuffixesPositions.end())
    {
      int commonLength = sparseTableRMQ.getMinOnRange(position, *greaterSuffixPositionIterator);
      maxLength = std::max(maxLength, commonLength);
    }
    maxLengths.push_back(maxLength);
    processedSuffixesPositions.insert(position);
  }
  return maxLengths;
}

void readInput(std::string* string)
{
  std::cin >> *string;
}

void printOutput(const std::vector<int>& maxLengths)
{
  for (int maxLength : maxLengths)
  {
    std::cout << maxLength << std::endl;
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::string string;
  readInput(&string);
  printOutput(findMaxLengths(string));
  return 0;
}
