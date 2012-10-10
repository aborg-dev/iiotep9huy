#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <string>
#include <limits>
#include <stdexcept>

using std::vector;

void readInput(vector<int>* array, vector<int>* query)
{
  int arraySize;
  std::cin >> arraySize;
  array->assign(arraySize, 0);
  for (int i = 0; i < arraySize; ++i)
  {
    std::cin >> (*array)[i];
  }
  int querySize;
  std::cin >> querySize;
  query->assign(querySize, 0);
  for (int position = 0; position < querySize; ++position)
  {
    std::string symbol;
    std::cin >> symbol;
    if (symbol == "L")
    {
      (*query)[position] = 0;
    }
    else
    if (symbol == "R")
    {
      (*query)[position] = 1;
    }
  }
}

class StackWithMaximum
{
public:
  StackWithMaximum()
  {
    data.push(std::numeric_limits<int>::min());
    max.push(std::numeric_limits<int>::min());
  }
  void pop()
  {
    data.pop();
    max.pop();
  }
  int top() const
  {
    return data.top();
  }
  int getMax() const
  {
    return max.top();
  }
  void push(int value)
  {
    int newMax = std::max(value, max.top());
    data.push(value);
    max.push(newMax);
  }
  bool empty() const
  {
    return (data.size() == 1);
  }

private:
  std::stack<int> data;
  std::stack<int> max;
};

class QueueWithMaximum
{
public:
  void enqueue(int value)
  {
    in.push(value);
  }
  void dequeue()
  {
    if (out.empty())
    {
      while (!in.empty())
      {
        out.push(in.top());
        in.pop();
      }
    }
    out.pop();
  }
  int getMax() const
  {
    return std::max(in.getMax(), out.getMax());
  }
  
private:
  StackWithMaximum in, out;
};

vector<int> getQueriesAnswers(const vector<int>& array, const vector<int>& query)
{
  vector<int> answer(query.size());
  QueueWithMaximum queue;
  int left = 0;
  int right = 0;
  queue.enqueue(array[left]);
  for (int queryNumber = 0; queryNumber < query.size(); ++queryNumber)
  {
    if (query[queryNumber] == 0)
    {
      ++left;
      queue.dequeue();
    }
    else
    {
      ++right;
      queue.enqueue(array[right]);
    }
    answer[queryNumber] = queue.getMax();
  }
  return answer;
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
  vector<int> array, query;
  readInput(&array, &query);
  vector<int> answer = getQueriesAnswers(array, query);
  printOutput(answer);
  return 0;
}
