#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>

#include "utils/Utils.hpp"

std::vector<int> buildPrefixFunction(const std::string& s)
{
  std::vector<int> prefixFunction(s.length());
  prefixFunction[0] = 0;
  int matchLenght = 0;
  for(int i = 1; i < s.length(); i++)
  {
    while (matchLenght > 0 && s[matchLenght] != s[i])
      matchLenght = prefixFunction[matchLenght - 1];
    if (s[matchLenght] == s[i])
      matchLenght++;
    prefixFunction[i] = matchLenght;
  }
  return prefixFunction;
}

void print(const std::vector<int>& v)
{
  for(int i = 0; i < v.size(); i++)
  {
    printf("%d ", v[i]);
  }
  printf("\n");
}

std::vector<int> findMatches(const std::string& text, const std::string& pattern)
{
  std::vector<int> matches;
  std::string merged = pattern + '$' + text;
  std::vector<int> prefixFunction = buildPrefixFunction(merged);
  for(int i = pattern.length(); i < merged.length(); i++)
  {
    if (prefixFunction[i] >= pattern.length())
      matches.push_back(i - 2 * pattern.length());
  }
  return matches;
}

std::vector<int> findMatchesAlternative(const std::string& text, const std::string& pattern)
{
  std::vector<int> matches;
  std::vector<int> prefixFunction = buildPrefixFunction(pattern);
  int matchLength = 0;
  for(int i = 0; i < text.length(); i++)
  {
    while (matchLength > 0 && pattern[matchLength] != text[i])
      matchLength = prefixFunction[matchLength - 1];

    if (pattern[matchLength] == text[i])
      matchLength++;

    if (matchLength == pattern.length())
    {
      matches.push_back(i - pattern.length() + 1);
      matchLength = prefixFunction[matchLength - 1];
    }
  }
  return matches;
}

char randLetter()
{
  return rand()%26 + 'a';
}

void generateInput()
{
  freopen("input.txt", "w", stdout);
  int textLength = 100000000;
  int patternLength = 1000;
  for(int i = 0; i < textLength; i++)
  {
    printf("%c", randLetter());
  }
  printf("\n");
  for(int i = 0; i < patternLength; i++)
  {
    printf("%c", randLetter());
  }
  printf("\n");
}

void testAlgorithms(const std::string& text, const std::string& pattern)
{
  {
    Timer t("findMatches");
    t.start();
    print(findMatches(text, pattern));
    t.printTime();
  }
  {
    Timer t("findMatchesAlternative");
    t.start();
    print(findMatchesAlternative(text, pattern));
    t.printTime();
  }
}

int main()
{
  generateInput();
  //return 0;
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  std::string text, pattern;
  std::cin >> text >> pattern;
  testAlgorithms(text, pattern);
  return 0;
}

