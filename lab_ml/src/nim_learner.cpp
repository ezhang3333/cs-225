/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "nim_learner.h"
#include <ctime>


/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) 
{
    for(int tokens = startingTokens; tokens >= 0; --tokens)
    {
      g_.insertVertex("p1-" + std::to_string(tokens));
      g_.insertVertex("p2-" + std::to_string(tokens));
    }

    for(int tokens = startingTokens; tokens > 0; --tokens)
    {
      std::string current = "p1-" + std::to_string(tokens);
      if (tokens >= 1) g_.insertEdge(current, "p2-" + std::to_string(tokens - 1));
      if (tokens >= 2) g_.insertEdge(current, "p2-" + std::to_string(tokens - 2));

      current = "p2-" + std::to_string(tokens);
      if (tokens >= 1) g_.insertEdge(current, "p1-" + std::to_string(tokens - 1));
      if (tokens >= 2) g_.insertEdge(current, "p1-" + std::to_string(tokens - 2));
    }

    for(Edge &edge : g_.getEdges())
    {
      g_.setEdgeWeight(edge.source, edge.dest, 0);
    }

    startingVertex_ = "p1-" + std::to_string(startingTokens);
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const 
{
std::vector<Edge> path;
    
    // Start at the initial state
    std::string currentVertex = startingVertex_;
    int tokens = std::stoi(currentVertex.substr(3));  // Extract the token count from the starting vertex label
    
    // Continue until we reach a state with 0 tokens
    while (tokens > 0) {
        int move = (tokens == 1) ? 1 : (rand() % 2) + 1;  // Take either 1 or 2 tokens, depending on availability
        int nextTokens = tokens - move;
        
        // Determine the next player
        std::string nextPlayer = (currentVertex[1] == '1') ? "p2-" : "p1-";
        std::string nextVertex = nextPlayer + std::to_string(nextTokens);
        
        // Get the edge and add it to the path
        Edge edge = g_.getEdge(currentVertex, nextVertex);
        path.push_back(edge);
        
        // Update the current state
        currentVertex = nextVertex;
        tokens = nextTokens;
    }
    
    return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) 
{
      // Determine the winner based on the last vertex in the path
    bool player1Won = (path.back().dest == "p2-0");

    // Iterate through each edge in the path
    for (const Edge & edge : path) {
        int currentWeight = g_.getEdgeWeight(edge.source, edge.dest);

        // If Player 1 won, reward Player 1's moves and punish Player 2's moves
        if (player1Won) {
            if (edge.source[1] == '1') {
                // Player 1's move, reward it
                g_.setEdgeWeight(edge.source, edge.dest, currentWeight + 1);
            } else {
                // Player 2's move, punish it
                g_.setEdgeWeight(edge.source, edge.dest, currentWeight - 1);
            }
        } else {
            // Player 2 won, so reward Player 2's moves and punish Player 1's moves
            if (edge.source[1] == '2') {
                // Player 2's move, reward it
                g_.setEdgeWeight(edge.source, edge.dest, currentWeight + 1);
            } else {
                // Player 1's move, punish it
                g_.setEdgeWeight(edge.source, edge.dest, currentWeight - 1);
            }
        }
    }
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold)           { g_.setEdgeLabel(v, w, "WIN"); }
      else if (weight < -1 * threshold) { g_.setEdgeLabel(v, w, "LOSE"); }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
