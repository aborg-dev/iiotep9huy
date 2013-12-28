#include <iostream>
#include <limits>
#include <vector>
#include <deque>

struct Edge
{
  Edge(int destination, int length): destination(destination), length(length) {}
  Edge(): Edge(0, 0) {}

  int destination, length;
};

const int NOT_REACHABLE = -1;

class Graph
{
public:
  Graph() {}
  explicit Graph(int verticesNumber): incidenceList(verticesNumber) {}

  void addEdge(int source, int destination, int length)
  {
    incidenceList[source].push_back(Edge(destination, length));
  }

  int getVerticesNumber() const
  {
    return incidenceList.size();
  }

  const std::vector<Edge>& getVertexIncidence(int vertex) const
  {
    return incidenceList.at(vertex);
  }

private:
  std::vector<std::vector<Edge>> incidenceList;
};

int findShortestDistance(int source, int destination, const Graph& graph)
{
  int verticesNumber = graph.getVerticesNumber();
  std::vector<int> distance(verticesNumber, verticesNumber + 1);
  distance[source] = 0;
  std::deque<int> orderOfVerticesProcessing;
  orderOfVerticesProcessing.push_front(source);

  while (!orderOfVerticesProcessing.empty())
  {
    size_t currentVertex = orderOfVerticesProcessing.front();
    orderOfVerticesProcessing.pop_front();

    for (const Edge& edge : graph.getVertexIncidence(currentVertex))
    {
      if (distance[currentVertex] + edge.length < distance[edge.destination])
      {
        distance[edge.destination] = distance[currentVertex] + edge.length;
        if (edge.length == 0)
        {
          orderOfVerticesProcessing.push_front(edge.destination);
        }
        else
        {
          orderOfVerticesProcessing.push_back(edge.destination);
        }
      }
    }
  }
  if (distance[destination] == verticesNumber + 1)
  {
    return NOT_REACHABLE;
  }
  else
  {
    return distance[destination];
  }
}

void readInput(int& source, int& destination, Graph* graph)
{
  int verticesNumber, edgesNumber;
  std::cin >> verticesNumber >> edgesNumber >> source >> destination;
  source--;
  destination--;
  *graph = Graph(verticesNumber);
  for (int i = 0; i < edgesNumber; ++i)
  {
    int src, dest, len;
    std::cin >> src >> dest >> len;
    src--;
    dest--;
    graph->addEdge(src, dest, len);
    graph->addEdge(dest, src, len);
  }
}

void printOutput(int distance)
{
  if (distance == NOT_REACHABLE)
  {
    std::cout << -1 << std::endl;
  } 
  else 
  {
    std::cout << distance << std::endl;
  }
}

int main()
{
  int source, destination;
  Graph graph;
  readInput(source, destination, &graph);
  printOutput(findShortestDistance(source, destination, graph));
  return 0;
}
