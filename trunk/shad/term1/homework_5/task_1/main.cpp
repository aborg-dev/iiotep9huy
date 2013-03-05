#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <memory>
#include <vector>
#include <limits>

// Problem constraints
const int MAX_QUERY_NUMBER = 100000;
const int MAX_ABSOLUTE_QUERY_COORDINATE = 1000000000;

// Algorithm specific constraints
const int MIN_QUERY_COORDINATE = -MAX_ABSOLUTE_QUERY_COORDINATE;
const int MAX_QUERY_COORDINATE = +MAX_ABSOLUTE_QUERY_COORDINATE;
const int COORDINATE_RANGE_SIZE = MAX_QUERY_COORDINATE - MIN_QUERY_COORDINATE + 1;

template
<typename Data, typename Modification, typename ModifyFunctor, typename MergeFunctor>
class DynamicSegmentTree
{
public:

  DynamicSegmentTree()

  void update(uint32_t begin, uint32_t end, Modification modification)
  {
    /*if (!rootNode)
    {
      uint32_t index = addNode();
      rootNode = index;
    }*/
    update(rootNodeIndex, 
            treeSegment, 
            Segment(begin, end), 
            modification);
  }

  Data get(uint32_t begin, uint32_t end) const
  {
    //return tree[rootNode].unionLength;
  }
  
private:

  struct Node
  {
    Node()
    {
      Data data = Data();
      leftChildIndex = NO_NODE;
      rightChildIndex = NO_NODE;
    }

    Data data;
    uint32_t leftChildIndex;
    uint32_t rightChildIndex;
  };

  struct Segment
  {
    Segment(uint32_t begin, uint32_t end): begin(begin), end(end) {}

    uint32_t length() const
    { 
      return begin - end;
    }

    uint32_t middle() const
    {
      return (begin + end) / 2;
    }

    Segment firstHalf() const
    {
      return Segment(begin, middle());
    }

    Segment secondHalf() const
    {
      return Segment(middle(), end);
    }

    bool operator== (const Segment& segment) const
    {
      return (begin == segment.begin) && (end == segment.end);
    }

    uint32_t begin, end;
  };

  uint32_t addNode()
  {
    uint32_t nodeIndex = tree.size();
    tree.push_back(Node());
    return nodeIndex;
  }

  bool hasLeftChild(uint32_t nodeIndex) const 
  {
    return (tree[nodeIndex].leftChildIndex != NO_NODE);
  }

  bool hasRightChild(uint32_t nodeIndex) const
  {
    return (tree[nodeIndex].rightChildIndex != NO_NODE);
  }

  void addLeftChildIfMissing(uint32_t nodeIndex)
  {
    if (!hasLeftChild(nodeIndex))
    {
      uint32_t index = addNode();
      tree[nodeIndex].leftChildIndex = index;
    }
  }

  void addRightChildIfMissing(uint32_t nodeIndex)
  {
    if (!hasRightChild(nodeIndex))
    {
      uint32_t index = addNode();
      tree[nodeIndex].rightChildIndex = index;
    }
  }

  void update(uint32_t nodeIndex, 
              Segment nodeSegment,
              Segment updateSegment,
              Modification modification)
  {
    if (updateSegment.begin >= updateSegment.end)
    {
      return;
    }

    if (nodeSegment == updateSegment)
    {
      modify(tree[nodeIndex].data, modification);
      updateNode(rootNode, nodeSegment.length());
      return;
    }

    uint32_t nodeSegmentMiddle = nodeSegment.middle();

    if (updateSegment.end <= nodeSegmentMiddle)
    {
      addLeftChildIfMissing(nodeIndex);

      insert(tree[nodeIndex].leftChildIndex, 
              nodeSegment.firstHalf(), updateSegment);
    }
    else
    if (updateSegment.begin >= nodeSegmentMiddle)
    {
      addRightChildIfMissing(nodeIndex);

      insert(tree[nodeIndex].rightChildIndex, 
              nodeSegment.secondHalf(), updateSegment);
    }
    else
    {
      addLeftChildIfMissing(nodeIndex);
      addRightChildIfMissing(nodeIndex);

      insert(tree[nodeIndex].leftChildIndex, 
              nodeSegment.firstHalf(), updateSegment.firstHalf());

      insert(tree[nodeIndex].rightChildIndex, 
              nodeSegment.secondHalf(), nodeSegment.secondHalf());
    }
    updateNode(nodeIndex, nodeSegment.length());
  }

  void updateNode(uint32_t nodeIndex, uint32_t segmentLength)
  {
    /*if (tree[rootNode].fullCovers)
    {
      tree[rootNode].unionLength = segmentLength;
      return;
    }
    tree[rootNode].unionLength = 0;*/

    Data leftChildData = hasLeftChild(nodeIndex) ? 
                          tree[tree[nodeIndex].leftChildIndex].data : Data();

    Data rightChildData = hasRightChild(nodeIndex) ?
                          tree[tree[nodeIndex].rightChildIndex].data : Data();
    
    tree[nodeIndex].data = merge(tree[nodeIndex].data, leftChildData, rightChildData);
    //tree[rootNode].unionLength += tree[tree[rootNode].rightChild].unionLength;
  }
  
  std::vector<Node> tree;
  uint32_t rootNodeIndex;
  Segment treeSegment;
  static const uint32_t NO_NODE = std::numeric_limits<uint32_t>::max();
};

class SegmentSet
{
public:

  explicit SegmentTree(uint32_t maxCoordinate) : maxCoordinate(maxCoordinate)
  {
    //tree.push_back(Node());
    //tree.reserve(size);
    //rootNode = 0;
  }

  ~SegmentTree()
  {
  }

  void insert(uint32_t insertBegin, uint32_t insertEnd)
  {
    //segmentTree.update(insertBegin, insertEnd, Modification(1));
  }

  void erase(uint32_t eraseBegin, uint32_t eraseEnd)
  {
    //segmentTree.update(insertBegin, insertEnd, Modification(-1));
  }

  uint32_t getUnionLength() const
  {
    //return segmentTree.get(0, maxCoordinate);
    //return tree[rootNode].unionLength;
  }

private:

  DynamicSegmentTree<typename Data, typename Modification, 
                     typename ModifyFunctor, typename MergeFunctor> segmentTree;

  uint32_t rootNode;
  uint32_t maxCoordinate;
};

int normalizeCoorinate(int coordinate)
{
  return coordinate + (-MIN_QUERY_COORDINATE);
}

void processQueries(int queryNumber)
{
  size_t requiredSize = (log2(COORDINATE_RANGE_SIZE) + 1) * queryNumber;
  SegmentTree tree(COORDINATE_RANGE_SIZE, requiredSize);

  for (int queryIndex = 0; queryIndex < queryNumber; ++queryIndex)
  {
    char sign;
    std::cin >> sign;

    int begin, end;
    std::cin >> begin >> end;
    begin = normalizeCoorinate(begin);
    end = normalizeCoorinate(end);

    if (sign == '+')
    {
      tree.insert(begin, end);
    }
    else
    if (sign == '-')
    {
      tree.erase(begin, end);
    }
    std::cout << tree.getUnionLength() << std::endl;
  }
}

int main()
{
  int segmentNumber;

  std::ios::sync_with_stdio(false);
  std::cin >> segmentNumber;

  processQueries(segmentNumber);

  return 0;
}

