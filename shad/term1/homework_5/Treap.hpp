#pragma once

#include <vector>
#include <functional>
#include <cstdlib>
#include <iostream>

template <typename Data, typename Key, typename KeyComparator = std::less<Key> >
class Treap
{
public:
  Treap()
  {
    tree.resize(1);
    treeRootIndex = 0;
  }

  Treap(std::vector< std::pair<Key, Data> > elements) 
  {
  }

  void insert(std::pair<Key, Data> element)
  {
    Node node(element.first, element.second);
    size_t nodeIndex = tree.size();
    tree.push_back(node);

    if (tree.size() == 2)
    {
      treeRootIndex = nodeIndex;
      return;
    }

    size_t leftSplitRootIndex, rightSplitRootIndex;

    split(node.key, treeRootIndex, leftSplitRootIndex, rightSplitRootIndex);

    leftSplitRootIndex = merge(leftSplitRootIndex, nodeIndex);
    treeRootIndex = merge(leftSplitRootIndex, rightSplitRootIndex);
  }

  void printTree()
  {
    printTree(treeRootIndex);
    std::cout << std::endl;
  }
  
private:
  struct Node
  {
    Node(const Key& key = Key(), const Data& data = Key()) : key(key), data(data)
    {
      leftChildIndex = rightChildIndex = 0;
      priority = rand();
    }

    Key key;
    Data data;
    size_t priority;

    size_t leftChildIndex, rightChildIndex;
  };

  void printTree(size_t rootIndex)
  {
    if (!rootIndex)
    {
      return;
    }

    printTree(tree[rootIndex].leftChildIndex);
    std::cout << "data: " << tree[rootIndex].data << " key: " << tree[rootIndex].key << std::endl;
    printTree(tree[rootIndex].rightChildIndex);
  }

  void split(Key splitKey, size_t rootIndex, size_t& leftSplitRootIndex, size_t& rightSplitRootIndex)
  {
    if (!rootIndex)
    {
      leftSplitRootIndex = rightSplitRootIndex = 0;
      return;
    }

    Node& rootNode = tree[rootIndex];
    size_t& leftChildIndex = rootNode.leftChildIndex;
    size_t& rightChildIndex = rootNode.rightChildIndex;

    if (keyComparator(splitKey, rootNode.key))
    {
      split(splitKey, leftChildIndex, leftSplitRootIndex, leftChildIndex);
      rightSplitRootIndex = rootIndex;
    }
    else
    {
      split(splitKey, rightChildIndex, rightChildIndex, rightSplitRootIndex);
      leftSplitRootIndex = rootIndex;
    }
  }

  size_t merge(size_t leftRootIndex, size_t rightRootIndex)
  {
    if (!leftRootIndex)
    {
      return rightRootIndex;
    }
    if (!rightRootIndex)
    {
      return leftRootIndex;
    }

    Node& leftRootNode = tree[leftRootIndex];
    Node& rightRootNode = tree[rightRootIndex];

    if (leftRootNode.priority < rightRootNode.priority)
    {
      leftRootNode.rightChildIndex = merge(leftRootNode.rightChildIndex, rightRootIndex);
      return leftRootIndex;
    }
    else
    {
      rightRootNode.leftChildIndex = merge(leftRootIndex, rightRootNode.leftChildIndex);
      return rightRootIndex;
    }
  }

  std::vector<Node> tree;
  size_t treeRootIndex;

  KeyComparator keyComparator;
  
}; // class Treap
