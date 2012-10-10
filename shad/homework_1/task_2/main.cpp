#include <iostream>
#include <algorithm>
#include <stack>
#include <string>
#include <stdexcept>

using std::string;

void readInput(string* brackets)
{
  std::cin.sync_with_stdio(false);
  std::cin >> *brackets;
}

bool isOpeningBracket(char bracket)
{
  return (bracket == '(' || bracket == '[' || bracket == '{');
}

bool isClosingBracket(char bracket)
{
  return (bracket == ')' || bracket == ']' || bracket == '}');
}

bool isMatchingPair(char openingBracket, char closingBracket)
{
  if (openingBracket == '(' && closingBracket == ')')
  {
    return true;
  }
  if (openingBracket == '[' && closingBracket == ']')
  {
    return true;
  }
  if (openingBracket == '{' && closingBracket == '}')
  {
    return true;
  }
  return false;
}

bool isCorrectBracketSequence(const string& brackets, int& maxCorrectPrefixLength)
{
  std::stack<char> bracketsStack;
  for (int position = 0; position < brackets.length(); ++position)
  {
    if (isOpeningBracket(brackets[position]))
    {
      bracketsStack.push(brackets[position]);
    }
    else if (isClosingBracket(brackets[position]))
    {
      if (!bracketsStack.empty() && isMatchingPair(bracketsStack.top(), brackets[position]))
      {
        bracketsStack.pop();
      }
      else
      {
        maxCorrectPrefixLength = position;
        return false;
      }
    }
    else
      throw std::logic_error("Incorrect character");
  }
  maxCorrectPrefixLength = brackets.length();
  return bracketsStack.empty();
}

int main()
{
  string brackets;
  readInput(&brackets);
  int maxCorrectPrefixLength = 0;
  if (isCorrectBracketSequence(brackets, maxCorrectPrefixLength))
  {
    std::cout << "CORRECT" << std::endl;
  }
  else
  {
    std::cout << maxCorrectPrefixLength << std::endl;
  }
  return 0;
}
