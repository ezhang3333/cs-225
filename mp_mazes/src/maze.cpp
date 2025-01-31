// maze.cpp

#include "maze.h"
#include "dsets.h" // Include the DisjointSets header
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <stack>
#include <random>
#include <cassert>

// Direction vectors corresponding to RIGHT, DOWN, LEFT, UP
const int DX[4] = {1, 0, -1, 0}; // RIGHT, DOWN, LEFT, UP
const int DY[4] = {0, 1, 0, -1};

// Constructor
SquareMaze::SquareMaze() : width_(0), height_(0) {}

void SquareMaze::makeMaze(int width, int height) {
    // Set dimensions
    width_ = width;
    height_ = height;
    
    // Initialize walls
    right_.resize(height_);
    down_.resize(height_);
    for (int y = 0; y < height_; ++y) {
        right_[y].resize(width_, true); // Initialize all right walls to true
        down_[y].resize(width_, true);  // Initialize all down walls to true
    }

    // Initialize disjoint sets
    dset_.addelements(width_ * height_);
    
    // Create list of all possible walls to remove
    std::vector<std::pair<int, int>> walls;
    for(int y = 0; y < height_; ++y) {
        for(int x = 0; x < width_; ++x) {
            if(x < width_ -1)
                walls.emplace_back(y * width_ + x, y * width_ + (x+1)); // Right wall
            if(y < height_ -1)
                walls.emplace_back(y * width_ + x, (y+1) * width_ + x); // Down wall
        }
    }

    // Shuffle the walls randomly
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(walls.begin(), walls.end(), g);

    // Iterate through walls and remove if it doesn't create a cycle
    for(auto &[cell1, cell2] : walls) {
        if(dset_.find(cell1) != dset_.find(cell2)) {
            // Determine the direction of the wall
            int x1 = cell1 % width_;
            int y1 = cell1 / width_;
            int x2 = cell2 % width_;
            int y2 = cell2 / width_;
            
            if(x1 == x2) { // Down wall
                setWall(x1, y1, DOWN, false);
            }
            else { // Right wall
                setWall(x1, y1, RIGHT, false);
            }
            dset_.setunion(cell1, cell2);
        }
    }
}


bool SquareMaze::canTravel(int x, int y, int dir) const {
    if(x >= width_ || y >= height_ || x < 0 || y < 0) return false;
    
    if(dir == 0) { // RIGHT
        if(x + 1 >= width_ || right_[y][x] == true) return false;
        else return true;
    } 
    else if(dir == 1) { // DOWN
        if(y + 1 >= height_ || down_[y][x] == true) return false;
        else return true;
    } 
    else if(dir == 2) { // LEFT
        if(x - 1 < 0 || right_[y][x - 1] == true) return false;
        else return true;
    } 
    else if(dir == 3) { // UP
        if(y - 1 < 0 || down_[y - 1][x] == true) return false;
        else return true;
    }
    return false;
}

// setWall implementation
void SquareMaze::setWall(int x, int y, Direction dir, bool exists) {
    if(x < 0 || x >= width_ || y < 0 || y >= height_)
        return;

    if(dir == RIGHT) {
        if(exists)
            maze_[y * width_ + x] |= 0x01; // Set right wall
        else
            maze_[y * width_ + x] &= ~0x01; // Remove right wall
    }
    else if(dir == DOWN) {
        if(exists)
            maze_[y * width_ + x] |= 0x02; // Set down wall
        else
            maze_[y * width_ + x] &= ~0x02; // Remove down wall
    }
}

// solveMaze implementation
std::vector<Direction> SquareMaze::solveMaze(int startX) {
    // BFS variables
    std::vector<int> distance(width_ * height_, -1);
    std::vector<int> predecessor(width_ * height_, -1);
    std::queue<int> q;

    int start = startX;
    distance[start] = 0;
    q.push(start);

    // BFS to compute distances
    while(!q.empty()) {
        int current = q.front();
        q.pop();

        int x = current % width_;
        int y = current / width_;

        for(int d = 0; d < 4; ++d) {
            Direction dir = static_cast<Direction>(d);
            if(canTravel(x, y, dir)) {
                int newX = x + DX[d];
                int newY = y + DY[d];
                int neighbor = newY * width_ + newX;
                if(distance[neighbor] == -1) {
                    distance[neighbor] = distance[current] + 1;
                    predecessor[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
    }

    // Find the cell in the bottom row with the maximum distance
    int maxDist = -1;
    int endCell = -1;
    for(int x = 0; x < width_; ++x) {
        int cell = (height_ -1) * width_ + x;
        if(distance[cell] > maxDist) {
            maxDist = distance[cell];
            endCell = cell;
        }
        else if(distance[cell] == maxDist && x < (endCell % width_)) {
            endCell = cell;
        }
    }

    // Reconstruct the path from endCell to start
    std::vector<Direction> path;
    int current = endCell;
    while(current != start) {
        int pred = predecessor[current];
        assert(pred != -1); // Ensure there is a predecessor
        int dx = (current % width_) - (pred % width_);
        int dy = (current / width_) - (pred / width_);
        if(dx == 1 && dy == 0)
            path.push_back(RIGHT);
        else if(dx == -1 && dy == 0)
            path.push_back(LEFT);
        else if(dx == 0 && dy == 1)
            path.push_back(DOWN);
        else if(dx == 0 && dy == -1)
            path.push_back(UP);
        current = pred;
    }

    // Reverse the path to start from the beginning
    std::reverse(path.begin(), path.end());
    return path;
}

// drawMaze implementation
cs225::PNG *SquareMaze::drawMaze(int start) const {
    if(width_ == 0 || height_ == 0)
        return nullptr;

    int imgWidth = width_ * 10 +1;
    int imgHeight = height_ *10 +1;
    cs225::PNG *png = new cs225::PNG(imgWidth, imgHeight);

    // Initialize all pixels to white
    for(int y = 0; y < imgHeight; ++y) {
        for(int x = 0; x < imgWidth; ++x) {
            cs225::HSLAPixel & pixel = png->getPixel(x, y);
            pixel.h = 0;
            pixel.s = 0;
            pixel.l = 1.0;
            pixel.a = 1.0;
        }
    }

    // Draw the top border (black) except for the entrance
    for(int x = 0; x < imgWidth; ++x) {
        if(x >= start *10 +1 && x < (start +1)*10)
            continue;
        cs225::HSLAPixel & pixel = png->getPixel(x, 0);
        pixel.h = 0;
        pixel.s = 0;
        pixel.l = 0;
        pixel.a = 1.0;
    }

    // Draw the left border (black)
    for(int y = 0; y < imgHeight; ++y) {
        cs225::HSLAPixel & pixel = png->getPixel(0, y);
        pixel.h = 0;
        pixel.s = 0;
        pixel.l = 0;
        pixel.a = 1.0;
    }

    // Draw the walls
    for(int y = 0; y < height_; ++y) {
        for(int x = 0; x < width_; ++x) {
            int cell = y * width_ + x;
            // Right wall
            if(maze_[cell] & 0x01) {
                for(int k = 0; k <=10; ++k) {
                    int px = (x +1)*10;
                    int py = y*10 +k;
                    if(px < imgWidth && py < imgHeight) {
                        cs225::HSLAPixel & pixel = png->getPixel(px, py);
                        pixel.h = 0;
                        pixel.s = 0;
                        pixel.l = 0;
                        pixel.a = 1.0;
                    }
                }
            }
            // Down wall
            if(maze_[cell] & 0x02) {
                for(int k = 0; k <=10; ++k) {
                    int px = x*10 +k;
                    int py = (y +1)*10;
                    if(px < imgWidth && py < imgHeight) {
                        cs225::HSLAPixel & pixel = png->getPixel(px, py);
                        pixel.h = 0;
                        pixel.s = 0;
                        pixel.l = 0;
                        pixel.a = 1.0;
                    }
                }
            }
        }
    }

    return png;
}

// drawMazeWithSolution implementation
cs225::PNG *SquareMaze::drawMazeWithSolution(int start) {
    // Draw the maze
    cs225::PNG *png = drawMaze(start);
    if(png == nullptr)
        return nullptr;

    // Solve the maze
    std::vector<Direction> solution = solveMaze(start);

    // Define the red color for the solution path
    cs225::HSLAPixel red;
    red.h = 0;
    red.s = 1.0;
    red.l = 0.5;
    red.a = 1.0;

    // Starting coordinates (center of the starting cell)
    int x = start * 10 + 5;
    int y = 5;

    // Draw the solution path
    for(auto dir : solution) {
        // Draw a segment of 10 pixels in the specified direction
        for(int i = 0; i < 10; ++i) {
            // Ensure coordinates are within bounds before coloring
            if(x < 0 || y < 0 || x >= static_cast<int>(width_) * 10 || y >= static_cast<int>(height_) * 10) {
                std::cerr << "Error: Path drawing out of bounds at (" << x << ", " << y << ")." << std::endl;
                assert(false); // Or handle the error gracefully
            }

            // Color the current pixel red
            cs225::HSLAPixel & pixel = png->getPixel(x, y);
            pixel = red;

            // Update coordinates based on direction
            if(dir == RIGHT)
                x++;
            else if(dir == DOWN)
                y++;
            else if(dir == LEFT && x > 0)
                x--;
            else if(dir == UP && y > 0)
                y--;
        }
    }

    // Determine the end cell based on the solution path
    int endX = start;
    int endY = 0;
    for(auto dir : solution) {
        endX += DX[dir];
        endY += DY[dir];
    }

    // Remove the down wall of the end cell to create an exit
    setWall(endX, endY, DOWN, false);

    // Draw the exit path (one final step down)
    for(int i = 0; i < 10; ++i) {
        int px = endX * 10 + 5; // Center of the cell
        int py = (endY + 1) * 10 + i;

        // Ensure exit pixels are within bounds
        if(px < static_cast<int>(png->width()) && py < static_cast<int>(png->height())) {
            cs225::HSLAPixel & pixel = png->getPixel(px, py);
            pixel = red; // Optionally, keep it red to show the exit path
        }
    }

    // Whiten the exit pixels to visually indicate the exit
    for(int k = 1; k < 10; ++k) {
        int px = endX * 10 + k;
        int py = (endY + 1) * 10;
        if(px < static_cast<int>(png->width()) && py < static_cast<int>(png->height())) {
            cs225::HSLAPixel & pixel = png->getPixel(px, py);
            pixel.h = 0;
            pixel.s = 0;
            pixel.l = 1.0;
            pixel.a = 1.0;
        }
    }

    return png;
}

