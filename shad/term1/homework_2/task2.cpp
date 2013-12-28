#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>

using std::vector;

struct Player
{
  int effect;
  int id;
};

struct effectComparator
{
  bool operator () (const Player& p1, const Player& p2)
  {
    return p1.effect > p2.effect;
  }
};

struct idComparator
{
  bool operator () (const Player& p1, const Player& p2)
  {
    return p1.id < p2.id;
  }
};

template
<typename Iterator, typename Comparator>
void mergeSort(Iterator begin, Iterator end, Comparator comparator)
{
  int length = end - begin;
  if (length <= 1)
    return;

  int M = (length / 2);
  mergeSort(begin, begin + M, comparator);
  mergeSort(begin + M, end, comparator);
  vector< std::remove_reference(decltype(*begin))::type > merged;
  Iterator first_it = begin;
  Iterator middle_it = begin + M;
  Iterator second_it = middle_it;

  while (first_it != middle_it && second_it != end)
  {
    if (comparator(*first_it, *second_it))
    {
      merged.push_back(*first_it);
      ++first_it;
    }
    else
    {
      merged.push_back(*second_it);
      ++second_it;
    }
  }
  std::copy(first_it, middle_it, merged.begin());
  std::copy(second_it, end, merged.begin());
  /*
  while (first_it != middle_it)
  {
    merged.push_back(*first_it);
    ++first_it;
  }
  while (second_it != end)
  {
    merged.push_back(*second_it);
    ++second_it;
  }
  */
  std::copy(merged.begin(), merged.end(), begin);
}

void readInput(vector<Player>* players)
{
  int playerNumber;
  std::cin >> playerNumber;
  players->resize(playerNumber);

  for (int i = 0; i < playerNumber; ++i)
  {
    std::cin >> (*players)[i].effect;
    (*players)[i].id = i + 1;
  }
}

void findEffectiveSubsequence(vector<Player> players, int& maxEffect, vector<Player>* subsequence)
{
  mergeSort(players.begin(), players.end(), effectComparator());
  int L = 0, R = 0;
  int bestL = 0, bestR = 0;
  maxEffect = 0;
  int currentEffect = 0;

  for (; R < players.size(); )
  {
    currentEffect += players[R].effect;
    ++R;
    while (R - L > 2 && (players[L].effect > players[R - 1].effect + players[R - 2].effect))
    {
      currentEffect -= players[L].effect;
      ++L;
    }
    if (currentEffect > maxEffect)
    {
      maxEffect = currentEffect;
      bestL = L;
      bestR = R;
    }
  }
  subsequence->resize(bestR - bestL);
  for (int i = bestL; i < bestR; ++i)
  {
    (*subsequence)[i] = players[i];
  }
}

void printOutput(int effect, const vector<Player>& subsequnce)
{
  std::cout << effect << std::endl;
  for (int i = 0; i < subsequnce.size(); ++i)
  {
    std::cout << subsequnce[i].id << " ";
  }
  std::cout << std::endl;
}

int main()
{
  vector<Player> players;
  readInput(&players);

  int effect;
  vector<Player> subsequence;

  findEffectiveSubsequence(players, effect, &subsequence);

  mergeSort(subsequence.begin(), subsequence.end(), idComparator());

  printOutput(effect, subsequence);
  return 0;
}
