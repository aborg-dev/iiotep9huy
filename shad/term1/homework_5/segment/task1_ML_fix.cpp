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
  }

  ~SegmentTree()
  {
    freeMemory(rootNode);
  }

  void insert(uint32_t insertBegin, uint32_t insertEnd)
  {
    if (!rootNode)
    {
      rootNode = new Node();
    }
    insert(rootNode, 0, 2000000000, insertBegin, insertEnd);
  }

  void erase(uint32_t eraseBegin, uint32_t eraseEnd)
  {
    if (!rootNode)
    {
      rootNode = new Node();
    }
    erase(rootNode, 0, 2000000000, eraseBegin, eraseEnd);
  }

  uint32_t getUnionLength()
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

    uint32_t fullCovers;
    uint32_t unionLength;
    Node* leftChild;
    Node* rightChild;
  };

  void freeMemory(Node* rootNode)
  {
    if (!rootNode)
    {
      return;
    }
    freeMemory(rootNode->leftChild);
    freeMemory(rootNode->rightChild);
    free(rootNode);
    rootNode = 0;
  }

  void insert(Node* rootNode, uint32_t nodeBegin, uint32_t nodeEnd, 
                              uint32_t insertBegin, uint32_t insertEnd)
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

    uint32_t nodeMiddle = (nodeBegin + nodeEnd) / 2;

    if (insertEnd <= nodeMiddle)
    {
      if (!rootNode->leftChild)
      {
        rootNode->leftChild = new Node();
      }
      insert(rootNode->leftChild, nodeBegin, nodeMiddle,
                                  insertBegin, insertEnd);
    }
    else
    if (insertBegin >= nodeMiddle)
    {
      if (!rootNode->rightChild)
      {
        rootNode->rightChild = new Node();
      }
      insert(rootNode->rightChild, nodeMiddle, nodeEnd,
                                    insertBegin, insertEnd);
    }
    else
    {
      if (!rootNode->leftChild)
      {
        rootNode->leftChild = new Node();
      }
      if (!rootNode->rightChild)
      {
        rootNode->rightChild = new Node();
      }
      insert(rootNode->leftChild, nodeBegin, nodeMiddle,
                                  insertBegin, nodeMiddle);

      insert(rootNode->rightChild, nodeMiddle, nodeEnd,
                                    nodeMiddle, insertEnd);
    }
    updateNode(rootNode, nodeEnd - nodeBegin);
  }

  void erase(Node* rootNode, uint32_t nodeBegin, uint32_t nodeEnd, 
                              uint32_t eraseBegin, uint32_t eraseEnd)
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

    uint32_t nodeMiddle = (nodeBegin + nodeEnd) / 2;

    if (eraseEnd <= nodeMiddle)
    {
      if (!rootNode->leftChild)
      {
        rootNode->leftChild = new Node();
      }
      erase(rootNode->leftChild, nodeBegin, nodeMiddle,
                                  eraseBegin, eraseEnd);
    }
    else
    if (eraseBegin >= nodeMiddle)
    {
      if (!rootNode->rightChild)
      {
        rootNode->rightChild = new Node();
      }
      erase(rootNode->rightChild, nodeMiddle, nodeEnd,
                                    eraseBegin, eraseEnd);
    }
    else
    {
      if (!rootNode->leftChild)
      {
        rootNode->leftChild = new Node();
      }
      if (!rootNode->rightChild)
      {
        rootNode->rightChild = new Node();
      }
      erase(rootNode->leftChild, nodeBegin, nodeMiddle,
                                  eraseBegin, nodeMiddle);

      erase(rootNode->rightChild, nodeMiddle, nodeEnd,
                                    nodeMiddle, eraseEnd);
    }
    updateNode(rootNode, nodeEnd - nodeBegin);
  }

  void updateNode(Node* rootNode, uint32_t segmentLength)
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
      if (rootNode->leftChild->unionLength == 0)
      {
        freeMemory(rootNode->leftChild);
        rootNode->leftChild = 0;
      }
    }
    if (rootNode->rightChild)
    {
      rootNode->unionLength += rootNode->rightChild->unionLength;
      if (rootNode->rightChild->unionLength == 0)
      {
        freeMemory(rootNode->rightChild);
        rootNode->rightChild = 0;
      }
    }
  }
  
  Node* rootNode;
};

int main()
{
   freopen("input.txt", "r", stdin);

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
    begin += 1000000000;
    end += 1000000000;
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
