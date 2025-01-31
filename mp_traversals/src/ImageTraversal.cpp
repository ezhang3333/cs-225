#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"
#include "ImageTraversal.h"

namespace Traversals 
{

  // Helper function to calculate color delta between two pixels
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h * h) + (s * s) + (l * l));
  }

  // DFS-specific functions
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    work_list.push_front(point); 
  }

  void dfs_pop(std::deque<Point> & work_list) {
    work_list.pop_front();
  }

  Point dfs_peek(std::deque<Point> & work_list) {
    if(!work_list.empty()) 
    {
      return work_list.front();
    }
    return Point(-1, -1);
  }

  // BFS-specific functions
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    work_list.push_back(point);  
  }

  void bfs_pop(std::deque<Point> & work_list) {
    work_list.pop_front();
    
  }

  Point bfs_peek(std::deque<Point> & work_list) {
    if(!work_list.empty()) 
    {
      return work_list.front();
    }
    return Point(-1, -1); 
  }

  // ImageTraversal Constructor
  ImageTraversal::ImageTraversal(const PNG &png, const Point& start, double tolerance, TraversalFunctions functions)
  {
    img_ = png;
    tol_ = tolerance;
    fns_ = functions;
    start_ = start;
  }

  // begin() - Returns an iterator at the starting point
  ImageTraversal::Iterator ImageTraversal::begin() {
    return Iterator(img_, tol_, fns_, start_);
  }

  // end() - Returns a sentinel end iterator
  ImageTraversal::Iterator ImageTraversal::end() {
    return ImageTraversal::Iterator();  // Default constructor signifies the end
  }

  // Iterator default constructor for end()
  ImageTraversal::Iterator::Iterator() 
  { }  // Sets traversal_ to nullptr for end

  // Iterator constructor for the starting point
  ImageTraversal::Iterator::Iterator(PNG &png, double tolerance, TraversalFunctions &functions, const Point& startPoint)
  {
    seen.resize(img.width());

    img = png;
    tol = tolerance;
    fns = functions;
    start = startPoint;

    fns.add(work_list_, start);
    curr = fns.peek(work_list_);

    for(unsigned int x = 0; x < img.width(); x++)
    {
      std::vector<bool> column(img.height());

      for(unsigned int y = 0; y < img.height(); y++)
      {
        column.push_back(false);
      }

      seen.push_back(column);
    }

    seen[start.x][start.y] = true;
  }


  // Iterator increment operator - Advances the traversal
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() 
  {
    if(!work_list_.empty())
    {
      seen[curr.x][curr.y] = true;

      while(work_list_.empty() == false && seen[fns.peek(work_list_).x][fns.peek(work_list_).y] == true)
      {
        fns.pop(work_list_);
      }

      Point up = Point(curr.x, curr.y - 1);
      Point down = Point(curr.x, curr.y + 1);
      Point right = Point(curr.x + 1, curr.y);
      Point left = Point(curr.x - 1, curr.y);
      double temp;

      if(curr.x < img.width() - 1)
      {
        unsigned rightT = seen[right.x][right.y];

        if(rightT == false)
        {
          if(calculateDelta(img.getPixel(start.x,start.y), img.getPixel(right.x, right.y)) < tol)
          {
            fns.add(work_list_, right);
          }

        }
      }
      if((curr.y < img.height()-1))
      {
        unsigned downT = seen[down.x][down.y];
        if(downT == false)
        {
          if(calculateDelta(img.getPixel(start.x,start.y),img.getPixel(down.x,down.y)) <  tol)
            fns.add(work_list_,down);

        }
      }
      if((curr.x > 0))
      {
        unsigned leftT = seen[left.x][left.y];
        if(leftT == false)
        {
          
          if(calculateDelta(img.getPixel(start.x,start.y),img.getPixel(left.x,left.y)) <  tol)
            fns.add(work_list_,left);
      
        }
      }
      if((curr.y > 0))
      {
        unsigned upT = seen[up.x][up.y];
        if(upT == false)
        {
          if(calculateDelta(img.getPixel(start.x,start.y),img.getPixel(up.x,up.y)) <  tol)
            fns.add(work_list_,up);
        }
      }

      curr = fns.peek(work_list_);
      return *this;
    }
    return *this;

  }
  

  // Iterator accessor operator - Returns the current point
  Point ImageTraversal::Iterator::operator*() {
    return curr;
  }

  // Iterator inequality operator - Checks if two iterators are not equal
  bool ImageTraversal::Iterator::operator!=(const Iterator & other)  {
    return !(work_list_ == other.work_list_);
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
    return work_list_.empty();
  }
  // namespace Traversals
}