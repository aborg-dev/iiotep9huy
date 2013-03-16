#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

void readInput(std::string* inputString)
{
  std::cin >> *inputString;
}

void writeOutput(const std::vector<size_t>& array)
{
  for (size_t i = 0; i < array.size(); ++i)
  {
    std::cout << array[i] << " ";
  }
  std::cout << std::endl;
}

std::vector<size_t> buildPrefixFunction(const std::string& string)
{
  std::vector<size_t> prefixFunction(string.length());
  size_t currentMatchLength = 0;
  for (size_t i = 1; i < string.length(); ++i)
  {
    while (currentMatchLength > 0 && 
            string[i] != string[currentMatchLength])
    {
      currentMatchLength = prefixFunction[currentMatchLength - 1];
    }
    if (string[i] == string[currentMatchLength])
    {
      ++currentMatchLength;
    }
    prefixFunction[i] = currentMatchLength;
  }
  return prefixFunction;
}

int main()
{
  std::ios::sync_with_stdio(false); 
  std::string inputString;
  readInput(&inputString);
  writeOutput(buildPrefixFunction(inputString));
  return 0;
}
