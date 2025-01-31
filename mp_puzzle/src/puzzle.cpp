#include "puzzle.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <stdexcept>

// Default constructor: initializes the puzzle to the solved state
PuzzleState::PuzzleState() {
    for (char i = 0; i < 15; ++i) {
        state_[i] = i + 1;
    }
    state_[15] = 0;
}

// Custom constructor: initializes the puzzle with a given state
PuzzleState::PuzzleState(const std::array<char, 16> state) {
    if (std::all_of(state.begin(), state.end(), [](char val) { return val >= 0 && val <= 15; }) &&
        std::count(state.begin(), state.end(), 0) == 1) {
        state_ = state;
    } else {
        state_.fill(0); // Invalid input, initialize with zeros
    }
}

// Convert the puzzle state to an array
std::array<char, 16> PuzzleState::asArray() const {
    return state_;
}

// Equality operator
bool PuzzleState::operator==(const PuzzleState &rhs) const {
    return state_ == rhs.state_;
}

// Inequality operator
bool PuzzleState::operator!=(const PuzzleState &rhs) const {
    return state_ != rhs.state_;
}

// Less-than operator (for use in std::map and std::set)
bool PuzzleState::operator<(const PuzzleState &rhs) const {
    return state_ < rhs.state_;
}

// Helper function to locate the empty tile (0)
int PuzzleState::findEmptyTile() const {
    auto it = std::find(state_.begin(), state_.end(), 0);
    return std::distance(state_.begin(), it);
}

PuzzleState PuzzleState::getNeighbor(Direction direction) const {
    int emptyTile = findEmptyTile();
    int row = emptyTile / 4;
    int col = emptyTile % 4;

    PuzzleState neighbor = *this;

    switch (direction) {
        case Direction::UP:
            if (row > 0) {
                std::swap(neighbor.state_[emptyTile], neighbor.state_[emptyTile - 4]);
            } else {
                return PuzzleState(std::array<char, 16>{0});
            }
            break;
        case Direction::DOWN:
            if (row < 3) {
                std::swap(neighbor.state_[emptyTile], neighbor.state_[emptyTile + 4]);
            } else {
                return PuzzleState(std::array<char, 16>{0});
            }
            break;
        case Direction::LEFT:
            if (col > 0) {
                std::swap(neighbor.state_[emptyTile], neighbor.state_[emptyTile - 1]);
            } else {
                return PuzzleState(std::array<char, 16>{0});
            }
            break;
        case Direction::RIGHT:
            if (col < 3) {
                std::swap(neighbor.state_[emptyTile], neighbor.state_[emptyTile + 1]);
            } else {
                return PuzzleState(std::array<char, 16>{0});
            }
            break;
    }

    return neighbor;
}



// Get all possible neighbors
std::vector<PuzzleState> PuzzleState::getNeighbors() const {
    std::vector<PuzzleState> neighbors;
    for (Direction dir : {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT}) {
        PuzzleState neighbor = getNeighbor(dir);
        if (neighbor.state_ != std::array<char, 16>{0}) { // Exclude invalid states
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

int PuzzleState::manhattanDistance(const PuzzleState desiredState /*= PuzzleState()*/) const {
    PuzzleState solvedState;  // Default is solved state if none provided
    const PuzzleState &goalState = (desiredState.asArray() == std::array<char, 16>{0}) ? solvedState : desiredState;

    int distance = 0;

    for (char tile = 1; tile <= 15; ++tile) { // Ignore tile 0 (empty space)
        int currIndex = std::find(state_.begin(), state_.end(), tile) - state_.begin();
        int goalIndex = std::find(goalState.state_.begin(), goalState.state_.end(), tile) - goalState.state_.begin();

        int currRow = currIndex / 4;
        int currCol = currIndex % 4;

        int goalRow = goalIndex / 4;
        int goalCol = goalIndex % 4;

        distance += std::abs(currRow - goalRow) + std::abs(currCol - goalCol);
    }

    return distance;
}



// Define a hash function for PuzzleState to use with unordered_set/unordered_map
struct PuzzleStateHash {
    std::size_t operator()(const PuzzleState &state) const {
        std::array<char, 16> arr = state.asArray();
        return std::hash<std::string>()(std::string(arr.begin(), arr.end()));
    }
};

std::vector<PuzzleState> solveBFS(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
    if (startState == desiredState) {
        if (iterations) *iterations = 1;
        return {startState};
    }

    std::queue<std::vector<PuzzleState>> queue;
    std::unordered_set<PuzzleState, PuzzleStateHash> visited;
    queue.push({startState});
    visited.insert(startState);

    size_t steps = 0;

    while (!queue.empty()) {
        ++steps; // Increment for every state popped from the queue
        auto path = queue.front();
        queue.pop();

        const PuzzleState &currentState = path.back();
        for (const PuzzleState &neighbor : currentState.getNeighbors()) {
            if (visited.find(neighbor) != visited.end()) continue;

            std::vector<PuzzleState> newPath = path;
            newPath.push_back(neighbor);

            if (neighbor == desiredState) {
                if (iterations) *iterations = steps;
                return newPath;
            }

            queue.push(newPath);
            visited.insert(neighbor);
        }
    }

    if (iterations) *iterations = steps;
    return {};
}



std::vector<PuzzleState> solveAstar(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
    if (startState == desiredState) {
        if (iterations) *iterations = 1;
        return {startState};
    }

    auto compare = [](const std::pair<PuzzleState, int> &a, const std::pair<PuzzleState, int> &b) {
        return a.second > b.second;
    };

    std::priority_queue<std::pair<PuzzleState, int>, std::vector<std::pair<PuzzleState, int>>, decltype(compare)> openSet(compare);
    std::unordered_set<PuzzleState, PuzzleStateHash> closedSet;
    std::map<PuzzleState, PuzzleState> cameFrom;
    std::map<PuzzleState, int> gScore;

    gScore[startState] = 0;
    openSet.push({startState, startState.manhattanDistance(desiredState)});

    size_t steps = 0;

    while (!openSet.empty()) {
        ++steps; // Increment for every state processed
        auto current = openSet.top().first;
        openSet.pop();

        if (current == desiredState) {
            std::vector<PuzzleState> path;
            while (current != startState) {
                path.push_back(current);
                current = cameFrom[current];
            }
            path.push_back(startState);
            std::reverse(path.begin(), path.end());
            if (iterations) *iterations = steps;
            return path;
        }

        closedSet.insert(current);

        for (const PuzzleState &neighbor : current.getNeighbors()) {
            if (closedSet.find(neighbor) != closedSet.end()) continue;

            int tentativeGScore = gScore[current] + 1;

            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                int fScore = tentativeGScore + neighbor.manhattanDistance(desiredState);
                openSet.push({neighbor, fScore});
            }
        }
    }

    if (iterations) *iterations = steps;
    return {};
}
