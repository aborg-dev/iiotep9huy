#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <string>
#include <limits>

enum class Movement
{
  LEFT, RIGHT
};

void readInput(std::vector<int>* array, std::vector<Movement>* windowMoves)
{
  int arraySize;
  std::cin >> arraySize;
  array->resize(arraySize);
  for (int i = 0; i < arraySize; ++i)
  {
    std::cin >> (*array)[i];
  }

  int windowMovesNumber;
  std::cin >> windowMovesNumber;
  windowMoves->resize(windowMovesNumber);
  for (int moveNumber = 0; moveNumber < windowMovesNumber; ++moveNumber)
  {
    std::string symbol;
    std::cin >> symbol;

    if (symbol == "L")
    {
      (*windowMoves)[moveNumber] = Movement::LEFT;
    }
    else
    if (symbol == "R")
    {
      (*windowMoves)[moveNumber] = Movement::RIGHT;
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

std::vector<int> getMaximumsInSlidingWindow(const std::vector<int>& array, 
                                            const std::vector<Movement>& windowMoves)
{
  std::vector<int> maximumsInWindow(windowMoves.size());
  QueueWithMaximum queue;
  int windowLeftMargin = 0;
  int windowRightMargin = 0;
  queue.enqueue(array[windowLeftMargin]);

  for (int moveNumber = 0; moveNumber < windowMoves.size(); ++moveNumber)
  {
    if (windowMoves[moveNumber] == Movement::LEFT)
    {
      ++windowLeftMargin;
      queue.dequeue();
    }
    else
    if (windowMoves[moveNumber] == Movement::RIGHT)
    {
      ++windowRightMargin;
      queue.enqueue(array[windowRightMargin]);
    }
    maximumsInWindow[moveNumber] = queue.getMax();
  }
  return maximumsInWindow;
}

void printOutput(const std::vector<int>& maximumsInWindow)
{
  for (int i = 0; i < maximumsInWindow.size(); ++i)
  {
    std::cout << maximumsInWindow[i] << " ";
  }
  std::cout << std::endl;
}

int main()
{
  std::vector<int> array;
  std::vector<Movement> windowMoves;

  readInput(&array, &windowMoves);
  std::vector<int> maximumsInWindow = getMaximumsInSlidingWindow(array, windowMoves);
  printOutput(maximumsInWindow);

  return 0;
}
