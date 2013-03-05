#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <utility>
#include <stack>
#include <limits>
#include <stdexcept>
#include <functional>
#include <memory.h>
#include <memory>
#include <string>
#include <math.h>

using std::vector;

void readInput(vector<int>* sequence)
{
  int sequenceSize;

  std::cin >> sequenceSize;
  sequence->resize(sequenceSize);

  for (int i = 0; i < sequenceSize; ++i)
  {
    std::cin >> (*sequence)[i];
  }
}

std::ostream& operator<< (std::ostream& stream, const vector<int>& vector)
{
  for (int i = 0; i < vector.size(); ++i)
  {
    stream << vector[i] << " ";
  }
  return stream;
}

void printOutput(const vector<int>& postorder, const vector<int>& inorder)
{
  std::cout << postorder << std::endl;
  std::cout << inorder << std::endl;
}

class SearchTree
{
public:
  SearchTree()
  {
  }

  void buildFromPreorder(vector<int> preorder)
  {
    vector<int> nextBiggerIndex(preorder.size(), -1);
    preorder.push_back(std::numeric_limits<int>::max());

    struct ElementInfo
    {
      ElementInfo(int value, int index): 
        value(value), index(index) 
      {
      }

      int value, index;
    };
    
    std::stack<ElementInfo> orderStack;

    for (int i = 0; i < preorder.size(); ++i)
    {
      while (!orderStack.empty() && orderStack.top().value <= preorder[i])
      {
        nextBiggerIndex[orderStack.top().index] = i;
        orderStack.pop(); 
      }

      orderStack.push(ElementInfo(preorder[i], i));
    }
    int lastNode = 0;
    restoreTree(0, 0, preorder.size() - 1, preorder, nextBiggerIndex, lastNode);
  }

  vector<int> buildPostorder() const
  {
    vector<int> postorder;
    buildPostorder(0, postorder);
    return postorder;
  }

  vector<int> buildInorder() const
  {
    vector<int> inorder;
    buildInorder(0, inorder);
    return inorder;
  }

private:

  void restoreTree(int currentNode, int begin, int end, const vector<int>& preorder, 
    const vector<int>& nextBiggerIndex, int& lastNode)
  {
    if (begin == end)
    {
      return;
    }

    tree.push_back(Node(preorder[begin]));
    lastNode++;
    int middle = nextBiggerIndex[begin];

    if (middle != begin + 1)
    {
      tree[currentNode].leftChild = lastNode;
      restoreTree(lastNode, begin + 1, middle, preorder, nextBiggerIndex, lastNode);
    }
    if (middle != end)
    {
      tree[currentNode].rightChild = lastNode;
      restoreTree(lastNode, middle, end, preorder, nextBiggerIndex, lastNode);
    }
  }

  void buildPostorder(int currentNode, vector<int>& postorder) const
  {
    if (currentNode == -1)
    {
      return;
    }
    
    buildPostorder(tree[currentNode].leftChild, postorder);
    buildPostorder(tree[currentNode].rightChild, postorder);
    postorder.push_back(tree[currentNode].key);
  }

  void buildInorder(int currentNode, vector<int>& inorder) const
  {
    if (currentNode == -1)
    {
      return;
    }
    
    buildInorder(tree[currentNode].leftChild, inorder);
    inorder.push_back(tree[currentNode].key);
    buildInorder(tree[currentNode].rightChild, inorder);
  }

  struct Node
  {
    explicit Node(int key) : key(key), parent(-1), leftChild(-1), rightChild(-1)
    {
    }

    int key;
    int parent;
    int leftChild;
    int rightChild;
  };

  vector<Node> tree;
};

int main()
{
  vector<int> preorder;
  readInput(&preorder);

  SearchTree searchTree;
  searchTree.buildFromPreorder(preorder);
  vector<int> postorder = searchTree.buildPostorder();
  vector<int> inorder = searchTree.buildInorder();

  printOutput(postorder, inorder);
  return 0;
}

