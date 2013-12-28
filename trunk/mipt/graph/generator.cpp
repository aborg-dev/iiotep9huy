#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

struct Edge
{
  Edge() 
  {
  }

  Edge(int from, int to) : from(from), to(to)
  {
  }

  int from, to;
};

struct Graph
{
  Graph(int n)
  {
    vNumber = n;
    edges.assign(n, vector<Edge> ());
  }
  vector< vector<Edge> > edges;
  int vNumber;
};

Graph generateGraph(int size, double edgeProbability)
{
  Graph g;
  for (int i = 0; i < 
  return g;
}


int main()
{
  return 0;
}
