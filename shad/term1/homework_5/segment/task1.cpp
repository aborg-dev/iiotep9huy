#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>

using std::cin;
using std::cout;
using std::endl;

class SegmentTree
{
public:
  SegmentTree()
  {
    rootNode = std::shared_ptr<Node> (new Node());
    cout << sizeof(rootNode) << endl;
    cout << sizeof(rootNode->fullCovers) << endl;
    cout << sizeof(*rootNode) << endl;
  }

  void insert(int insertBegin, int insertEnd)
  {
    insert(rootNode, -1000000000, 1000000000, insertBegin, insertEnd);
  }

  void erase(int eraseBegin, int eraseEnd)
  {
    erase(rootNode, -1000000000, 1000000000, eraseBegin, eraseEnd);
  }

  size_t getUnionLength()
  {
    return rootNode->unionLength;
  }

private:
  struct Node
  {
    Node()
    {
      fullCovers = 0;
      unionLength = 0;
    }

    size_t fullCovers;
    size_t unionLength;
    std::shared_ptr<Node> leftChild;
    std::shared_ptr<Node> rightChild;
  };

  void insert(std::shared_ptr<Node> rootNode, int nodeBegin, int nodeEnd, 
                              int insertBegin, int insertEnd)
  {
    if (insertBegin >= insertEnd)
    {
      return;
    }
    if (nodeBegin == insertBegin && nodeEnd == insertEnd)
    {
      rootNode->fullCovers++;
      updateNode(rootNode, nodeEnd - nodeBegin);
      return;
    }

    int nodeMiddle = (nodeBegin + nodeEnd) / 2;

    if (insertEnd <= nodeMiddle)
    {
      if (!rootNode->leftChild)
      {
        rootNode->leftChild = std::shared_ptr<Node> (new Node());
      }
      insert(rootNode->leftChild, nodeBegin, nodeMiddle,
                                  insertBegin, insertEnd);
    }
    else
    if (insertBegin >= nodeMiddle)
    {
      if (!rootNode->rightChild)
      {
        rootNode->rightChild = std::shared_ptr<Node> (new Node());
      }
      insert(rootNode->rightChild, nodeMiddle, nodeEnd,
                                    insertBegin, insertEnd);
    }
    else
    {
      if (!rootNode->leftChild)
      {
        rootNode->leftChild = std::shared_ptr<Node> (new Node());
      }
      if (!rootNode->rightChild)
      {
        rootNode->rightChild = std::shared_ptr<Node> (new Node());
      }
      insert(rootNode->leftChild, nodeBegin, nodeMiddle,
                                  insertBegin, nodeMiddle);

      insert(rootNode->rightChild, nodeMiddle, nodeEnd,
                                    nodeMiddle, insertEnd);
    }
    updateNode(rootNode, nodeEnd - nodeBegin);
  }

  void erase(std::shared_ptr<Node> rootNode, int nodeBegin, int nodeEnd, 
                              int eraseBegin, int eraseEnd)
  {
    if (eraseBegin >= eraseEnd)
    {
      return;
    }
    if (nodeBegin == eraseBegin && nodeEnd == eraseEnd)
    {
      rootNode->fullCovers--;
      updateNode(rootNode, nodeEnd - nodeBegin);
      return;
    }

    int nodeMiddle = (nodeBegin + nodeEnd) / 2;

    if (eraseEnd <= nodeMiddle)
    {
      if (!rootNode->leftChild)
      {
        rootNode->leftChild = std::shared_ptr<Node> (new Node());
      }
      erase(rootNode->leftChild, nodeBegin, nodeMiddle,
                                  eraseBegin, eraseEnd);
    }
    else
    if (eraseBegin >= nodeMiddle)
    {
      if (!rootNode->rightChild)
      {
        rootNode->rightChild = std::shared_ptr<Node> (new Node());
      }
      erase(rootNode->rightChild, nodeMiddle, nodeEnd,
                                    eraseBegin, eraseEnd);
    }
    else
    {
      if (!rootNode->leftChild)
      {
        rootNode->leftChild = std::shared_ptr<Node> (new Node());
      }
      if (!rootNode->rightChild)
      {
        rootNode->rightChild = std::shared_ptr<Node> (new Node());
      }
      erase(rootNode->leftChild, nodeBegin, nodeMiddle,
                                  eraseBegin, nodeMiddle);

      erase(rootNode->rightChild, nodeMiddle, nodeEnd,
                                    nodeMiddle, eraseEnd);
    }
    updateNode(rootNode, nodeEnd - nodeBegin);
  }

  void updateNode(std::shared_ptr<Node> rootNode, size_t segmentLength)
  {
    if (rootNode->fullCovers)
    {
      rootNode->unionLength = segmentLength;
      return;
    }
    rootNode->unionLength = 0;
    if (rootNode->leftChild)
    {
      rootNode->unionLength += rootNode->leftChild->unionLength;
    }
    if (rootNode->rightChild)
    {
      rootNode->unionLength += rootNode->rightChild->unionLength;
    }
  }
  
  std::shared_ptr<Node> rootNode;
};

int main()
{
  // freopen("input.txt", "r", stdin);

  int segmentNumber;
  SegmentTree tree;

  cin.sync_with_stdio(false);
  cin >> segmentNumber;
  for (int queryIndex = 0; queryIndex < segmentNumber; ++queryIndex)
  {
    char sign;
    cin >> sign;
    int begin, end;
    cin >> begin >> end;
    if (sign == '+')
    {
      tree.insert(begin, end);
    }
    else
    if (sign == '-')
    {
      tree.erase(begin, end);
    }
    cout << tree.getUnionLength() << endl;
  }

  return 0;
}
