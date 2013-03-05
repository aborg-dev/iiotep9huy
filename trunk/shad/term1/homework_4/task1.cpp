#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <utility>
#include <stack>
#include <stdexcept>
#include <functional>
#include <memory.h>
#include <memory>
#include <string>
#include <math.h>

#define DB(x) std::cerr << #x << ": " << x << std::endl;

using std::vector;
using std::string;
using std::cin;

void readInput(vector<int>* sequence)
{
  int sequenceSize;

  cin >> sequenceSize;
  sequence->resize(sequenceSize);

  for (int i = 0; i < sequenceSize; ++i)
  {
    cin >> (*sequence)[i];
  }
}

std::ostream& operator<< (std::ostream& stream, const std::vector<int>& vector)
{
  for (int i = 0; i < vector.size(); ++i)
  {
    stream << vector[i] << " ";
  }
  stream << std::endl;
  return stream;
}

void printOutput(const vector<int>& postorder, const vector<int>& inorder)
{
  std::cout << postorder;
  std::cout << inorder;
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
    preorder.push_back(1000000009);
    std::stack< std::pair<int, int> > orderStack;
    for (int i = 0; i < preorder.size(); ++i)
    {
      while (!orderStack.empty() && orderStack.top().second <= preorder[i])
      {
        nextBiggerIndex[orderStack.top().first] = i;
        orderStack.pop(); 
      }

      orderStack.push(std::make_pair(i, preorder[i]));
    }
    int lastNode = 0;
    restoreTree(0, 0, preorder.size() - 1, preorder, nextBiggerIndex, lastNode);
  }

  vector<int> buildPostorder()
  {
    vector<int> postorder;
    buildPostorder(0, postorder);
    return postorder;
  }

  vector<int> buildInorder()
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
      return;

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

  void buildPostorder(int currentNode, vector<int>& postorder)
  {
    if (currentNode == -1)
      return;
    
    buildPostorder(tree[currentNode].leftChild, postorder);
    buildPostorder(tree[currentNode].rightChild, postorder);
    postorder.push_back(tree[currentNode].key);
  }

  void buildInorder(int currentNode, vector<int>& inorder)
  {
    if (currentNode == -1)
      return;
    
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
  // freopen("input.txt", "r", stdin); // REMOVE

  vector<int> preorder;
  readInput(&preorder);

  SearchTree T;
  T.buildFromPreorder(preorder);
  vector<int> postorder = T.buildPostorder();
  vector<int> inorder = T.buildInorder();

  printOutput(postorder, inorder);
  return 0;
}
