#include <vector>
#include <algorithm>
#include <set>
#include <limits>

#include "cs225_graph/graph.h"
#include "cs225_graph/edge.h"

#include "NetworkFlow.h"

int min(int a, int b) {
  if (a<b)
    return a;
  else return b;
}

NetworkFlow::NetworkFlow(Graph & startingGraph, Vertex source, Vertex sink) :
  g_(startingGraph), residual_(Graph(true,true)), flow_(Graph(true,true)), source_(source), sink_(sink) {

  for (Vertex v : g_.getVertices()) {
    residual_.insertVertex(v);
    flow_.insertVertex(v);
  }

  for (Edge e : g_.getEdges()) {
    Vertex u = e.source;
    Vertex v = e.dest;
    int weight = e.getWeight();

    residual_.insertEdge(u, v);
    residual_.setEdgeWeight(u, v, weight);

    residual_.insertEdge(v, u);
    residual_.setEdgeWeight(v, u, 0);

    flow_.insertEdge(u, v);
    flow_.setEdgeWeight(u, v, 0);
  }

  maxFlow_ = 0;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the helper function.
   *
   * @param source  The starting (current) vertex
   * @param sink    The destination vertex
   * @param path    The vertices in the path
   * @param visited A set of vertices we have visited
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, 
  std::vector<Vertex> &path, std::set<Vertex> &visited) {

  if (visited.count(source) != 0)
    return false;
  visited.insert(source);

  if (source == sink) {
    return true;
  }

  vector<Vertex> adjs = residual_.getAdjacent(source);
  for(auto it = adjs.begin(); it != adjs.end(); it++) {
    if (visited.count(*it) == 0 && residual_.getEdgeWeight(source,*it) > 0) {
      path.push_back(*it);
      if (findAugmentingPath(*it,sink,path,visited))
        return true;
      else {
        path.pop_back();
      }
    }
  }

  return false;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the main function.  It initializes a set to keep track of visited vertices.
   *
   * @param source The starting (current) vertex
   * @param sink   The destination vertex
   * @param path   The vertices in the path
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, std::vector<Vertex> &path) {
   std::set<Vertex> visited;
   path.clear();
   path.push_back(source);
   return findAugmentingPath(source,sink,path,visited);
}

  /**
   * pathCapacity - Determine the capacity of a path in the residual graph.
   *
   * @param path   The vertices in the path
   */

int NetworkFlow::pathCapacity(const std::vector<Vertex> & path) const {
  int minCapacity = std::numeric_limits<int>::max();

  for (size_t i = 0; i < path.size() - 1; ++i) {
    int weight = residual_.getEdgeWeight(path[i], path[i + 1]);
    minCapacity = std::min(minCapacity, weight);
  }

  return minCapacity;
}

  /**
   * calculuateFlow - Determine the capacity of a path in the residual graph.
   * Sets the member function `maxFlow_` to be the flow, and updates the
   * residual graph and flow graph according to the algorithm.
   *
   * @return The network flow graph.
   */

const Graph & NetworkFlow::calculateFlow() {
  std::vector<Vertex> path;

  while (findAugmentingPath(source_, sink_, path)) {
    int capacity = pathCapacity(path);

    // Update flow and residual graphs
    for (size_t i = 0; i < path.size() - 1; ++i) {
      Vertex u = path[i];
      Vertex v = path[i + 1];

      // Update forward edge in flow graph
      if (flow_.edgeExists(u, v)) {
        flow_.setEdgeWeight(u, v, flow_.getEdgeWeight(u, v) + capacity);
      } else {
        flow_.setEdgeWeight(v, u, flow_.getEdgeWeight(v, u) - capacity);
      }

      // Update residual graph
      residual_.setEdgeWeight(u, v, residual_.getEdgeWeight(u, v) - capacity);
      residual_.setEdgeWeight(v, u, residual_.getEdgeWeight(v, u) + capacity);
    }

    // Increase max flow by path capacity
    maxFlow_ += capacity;
  }

  return flow_;
}

int NetworkFlow::getMaxFlow() const {
  return maxFlow_;
}

const Graph & NetworkFlow::getGraph() const {
  return g_;
}

const Graph & NetworkFlow::getFlowGraph() const {
  return flow_;
}

const Graph & NetworkFlow::getResidualGraph() const {
  return residual_;
}

