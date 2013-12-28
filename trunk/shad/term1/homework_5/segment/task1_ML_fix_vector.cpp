#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <math.h>
#include <memory>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

const int MAXN = 100000;
const int MAXW = 2 * 1000 * 1000 * 1000;

class SegmentTree
{
public:
  SegmentTree()
  {
    tree.push_back(Node());
    rootNode = 0;
  }

  explicit SegmentTree(uint32_t size)
  {
    tree.push_back(Node());
    rootNode = 0;
    tree.reserve(size);
  }

  ~SegmentTree()
  {
  }

  void insert(uint32_t insertBegin, uint32_t insertEnd)
  {
    if (!rootNode)
    {
      uint32_t index = addNode();
      rootNode = index;
    }
    insert(rootNode, 0, MAXW, insertBegin, insertEnd);
  }

  void erase(uint32_t eraseBegin, uint32_t eraseEnd)
  {
    if (!rootNode)
    {
      uint32_t index = addNode();
      rootNode = index;
    }
    erase(rootNode, 0, MAXW, eraseBegin, eraseEnd);
  }

  uint32_t getUnionLength()
  {
    return tree[rootNode].unionLength;
  }

private:
  struct Node
  {
    Node()
    {
      fullCovers = 0;
      unionLength = 0;
      leftChild = 0;
      rightChild = 0;
    }

    uint32_t fullCovers;
    uint32_t unionLength;
    uint32_t leftChild;
    uint32_t rightChild;
  };

  uint32_t addNode()
  {
    tree.push_back(Node());
    return (tree.size() - 1);
  }

  void addLeftChildIfMissing(uint32_t rootNode)
  {
    if (!tree[rootNode].leftChild)
    {
      uint32_t index = addNode();
      tree[rootNode].leftChild = index;
    }
  }

  void addRightChildIfMissing(uint32_t rootNode)
  {
    if (!tree[rootNode].rightChild)
    {
      uint32_t index = addNode();
      tree[rootNode].rightChild = index;
    }
  }

  void insert(uint32_t rootNode, uint32_t nodeBegin, uint32_t nodeEnd, 
                                uint32_t insertBegin, uint32_t insertEnd)
  {
    if (insertBegin >= insertEnd)
    {
      return;
    }
    if (nodeBegin == insertBegin && nodeEnd == insertEnd)
    {
      tree[rootNode].fullCovers++;
      updateNode(rootNode, nodeEnd - nodeBegin);
      return;
    }

    uint32_t nodeMiddle = (nodeBegin + nodeEnd) / 2;

    if (insertEnd <= nodeMiddle)
    {
      addLeftChildIfMissing(rootNode);

      insert(tree[rootNode].leftChild, nodeBegin, nodeMiddle,
                                      insertBegin, insertEnd);
    }
    else
    if (insertBegin >= nodeMiddle)
    {
      addRightChildIfMissing(rootNode);

      insert(tree[rootNode].rightChild, nodeMiddle, nodeEnd,
                                        insertBegin, insertEnd);

    }
    else
    {
      addLeftChildIfMissing(rootNode);
      addRightChildIfMissing(rootNode);

      insert(tree[rootNode].leftChild, nodeBegin, nodeMiddle,
                                      insertBegin, nodeMiddle);

      insert(tree[rootNode].rightChild, nodeMiddle, nodeEnd,
                                        nodeMiddle, insertEnd);
    }
    updateNode(rootNode, nodeEnd - nodeBegin);
  }

  void erase(uint32_t rootNode, uint32_t nodeBegin, uint32_t nodeEnd, 
                                uint32_t eraseBegin, uint32_t eraseEnd)
  {
    if (eraseBegin >= eraseEnd)
    {
      return;
    }
    if (nodeBegin == eraseBegin && nodeEnd == eraseEnd)
    {
      tree[rootNode].fullCovers--;
      updateNode(rootNode, nodeEnd - nodeBegin);
      return;
    }

    uint32_t nodeMiddle = (nodeBegin + nodeEnd) / 2;

    if (eraseEnd <= nodeMiddle)
    {
      addLeftChildIfMissing(rootNode);

      erase(tree[rootNode].leftChild, nodeBegin, nodeMiddle,
                                      eraseBegin, eraseEnd);
    }
    else
    if (eraseBegin >= nodeMiddle)
    {
      addRightChildIfMissing(rootNode);

      erase(tree[rootNode].rightChild, nodeMiddle, nodeEnd,
                                      eraseBegin, eraseEnd);
    }
    else
    {
      addLeftChildIfMissing(rootNode);
      addRightChildIfMissing(rootNode);

      erase(tree[rootNode].leftChild, nodeBegin, nodeMiddle,
                                      eraseBegin, nodeMiddle);

      erase(tree[rootNode].rightChild, nodeMiddle, nodeEnd,
                                        nodeMiddle, eraseEnd);
    }
    updateNode(rootNode, nodeEnd - nodeBegin);
  }

  void updateNode(uint32_t rootNode, uint32_t segmentLength)
  {
    if (tree[rootNode].fullCovers)
    {
      tree[rootNode].unionLength = segmentLength;
      return;
    }
    tree[rootNode].unionLength = 0;
    if (tree[rootNode].leftChild)
    {
      tree[rootNode].unionLength += tree[tree[rootNode].leftChild].unionLength;
    }
    if (tree[rootNode].rightChild)
    {
      tree[rootNode].unionLength += tree[tree[rootNode].rightChild].unionLength;
    }
  }
  
  std::vector<Node> tree;
  uint32_t rootNode;
};

int main()
{
  // freopen("input.txt", "r", stdin);

  int segmentNumber;

  cin.sync_with_stdio(false);
  cin >> segmentNumber;

  SegmentTree tree((log2(MAXW) + 1) * segmentNumber);

  for (int queryIndex = 0; queryIndex < segmentNumber; ++queryIndex)
  {
    char sign;
    cin >> sign;
    int begin, end;
    cin >> begin >> end;
    begin += (MAXW / 2);
    end += (MAXW / 2);
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
