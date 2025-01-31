/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second, int curDim)
{
    if(first[curDim] < second[curDim])
    {
      return true;
    }
    else if(first[curDim] > second[curDim])
    {
      return false;
    }
    else
    {
      return first < second;
    }
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest, const Point<Dim>& potential)
{
    double currBestDist = 0;
    double dist = 0;

    for(int i = 0; i < Dim; i++)
    {
      dist = target[i] - currentBest[i];
      dist = dist * dist;
      currBestDist += dist;
    }    

    double potentialBestDist = 0;
    double potentialDist = 0;

    for(int i = 0; i < Dim; i++)
    {
      potentialDist = target[i] - potential[i];
      potentialDist = potentialDist * potentialDist;
      potentialBestDist += potentialDist;
    }

    if(currBestDist == potentialBestDist)
    {
      return potential < currentBest;
    }
    else
    {
      return potentialBestDist < currBestDist;
    }
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    if (newPoints.empty())
    {
        root = nullptr;
        size = 0;
        return;
    }

    vector<Point<Dim>> points = newPoints;
    size = points.size();
    root = buildTree(points, 0, points.size() - 1, 0);
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other)
{
  if(other.root == nullptr)
  {
    this->root = nullptr;
    size = 0;
  }
  else
  {
    root = copy(other.root);
    size = other.size;
  }
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
    if (this != &rhs) 
    {
        clear(root);  // Free the current memory
        if (rhs.root != nullptr) 
        {
            root = copy(rhs.root);  // Deep copy the other tree
            size = rhs.size;
        } 
        else 
        {
            root = nullptr;
            size = 0;
        }
    }
    return *this;
}


template <int Dim>
KDTree<Dim>::~KDTree() 
{
  clear(root); 
}

template<int Dim>
void KDTree<Dim>::clear(typename KDTree<Dim>::KDTreeNode* root)
{
  if(root != nullptr)
  {
    clear(root->left);
    clear(root->right);
    delete root;
  }
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const {
    // Start recursive search from the root node, at depth 0
    return findNearestNeighborHelper(query, root, 0);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighborHelper(const Point<Dim>& query, typename KDTree<Dim>::KDTreeNode* node, int depth) const {
    if (node == nullptr) {
        // Return an invalid point or handle the edge case when there's no point
        return Point<Dim>();  // You can modify this as per your Point class's invalid state
    }

    // Determine which dimension to compare at this depth (cycle through dimensions)
    int curDim = depth % Dim;

    // Initialize the best point to be the current node's point
    Point<Dim> currentBest = node->point;
    KDTreeNode* firstSubtree = nullptr;
    KDTreeNode* secondSubtree = nullptr;

    // Traverse the tree to find the nearest neighbor in the appropriate subtree
    if (smallerDimVal(query, node->point, curDim)) {
        firstSubtree = node->left;
        secondSubtree = node->right;
    } else {
        firstSubtree = node->right;
        secondSubtree = node->left;
    }

    // Recursively search in the "best" subtree first
    if (firstSubtree != nullptr) {
        Point<Dim> bestInSubtree = findNearestNeighborHelper(query, firstSubtree, depth + 1);
        if (shouldReplace(query, currentBest, bestInSubtree)) {
            currentBest = bestInSubtree;
        }
    }

    // Calculate the squared distance from the query point to the current best point
    double radiusSquared = 0;
    for (int i = 0; i < Dim; ++i) {
        double diff = query[i] - currentBest[i];
        radiusSquared += diff * diff;
    }

    // Check if we need to explore the "other" subtree
    double splitDist = query[curDim] - node->point[curDim];
    if (splitDist * splitDist <= radiusSquared && secondSubtree != nullptr) {
        // If the split distance is within the radius, search the second subtree
        Point<Dim> bestInOtherSubtree = findNearestNeighborHelper(query, secondSubtree, depth + 1);
        if (shouldReplace(query, currentBest, bestInOtherSubtree)) {
            currentBest = bestInOtherSubtree;
        }
    }

    // Return the best point found
    return currentBest;
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    while (start < end) {
        RandIter pivot = std::prev(end);  
        
        RandIter mid = std::partition(start, pivot, [&](const auto& elem) 
        {
            return cmp(elem, *pivot); 
        });

        std::iter_swap(mid, pivot);  

        if (mid == k) 
        {
            return;  
        } 
        else if (mid < k)
        {
            start = std::next(mid);  
        } 
        else 
        {
            end = mid;  
        }
    }
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(vector<Point<Dim>>& points, int left, int right, int depth) 
{
    if (left > right) 
    {
        return nullptr;
    }

    int curDim = depth % Dim;
    int medianIdx = (left + right) / 2;

    auto cmp = [curDim](const Point<Dim>& a, const Point<Dim>& b) 
    {
        return smallerDimVal(a, b, curDim);
    };

    select(points.begin() + left, points.begin() + right + 1, points.begin() + medianIdx, cmp);
    KDTreeNode* node = new KDTreeNode(points[medianIdx]);

    node->left = buildTree(points, left, medianIdx - 1, depth + 1);
    node->right = buildTree(points, medianIdx + 1, right, depth + 1);

    return node;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy(typename KDTree<Dim>::KDTreeNode* subroot) 
{
    if (subroot == nullptr) 
    {
        return nullptr;
    }

    KDTreeNode* newNode = new KDTreeNode(subroot->point);
    newNode->left = copy(subroot->left);
    newNode->right = copy(subroot->right);

    return newNode;
}
