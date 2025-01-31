
/**
 * @file heap.cpp
 * Implementation of a heap class.
 */
#include <cstddef>

template <class T, class Compare>
class heap;

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    return 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    return currentIdx * 2;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    return (currentIdx * 2) + 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    return currentIdx / 2;
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    return leftChild(currentIdx) < _elems.size();
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    int right = rightChild(currentIdx);
    int left = leftChild(currentIdx);

    if(higherPriority(_elems[right],  _elems[left]))
    {
        return right;
    }
    else
    {
        return left;
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    if(!hasAChild(currentIdx)) return;

    int minChildIndex = maxPriorityChild(currentIdx);

    if(_elems[currentIdx] > _elems[minChildIndex])
    {
        std::swap(_elems[currentIdx], _elems[minChildIndex]);
        heapifyDown(minChildIndex);
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    if (currentIdx == root())
        return;
    size_t parentIdx = parent(currentIdx);
    if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
        std::swap(_elems[currentIdx], _elems[parentIdx]);
        heapifyUp(parentIdx);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{
    _elems.push_back(T());
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems) 
{
    _elems.push_back(T());

    for(T item : elems)
    {
        _elems.push_back(item);
    }

    int start = _elems.size() / 2;
    for(auto i = start; i > 0; i--)
    {
        heapifyDown(i);
    }
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    T temp = _elems[1];
    std::swap(_elems[1], _elems[_elems.size() - 1]);
    _elems.pop_back();

    heapifyDown(1);
    return temp;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    return _elems[1];
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    _elems.push_back(elem);
    heapifyUp(_elems.size() - 1);
}

template <class T, class Compare>
void heap<T, Compare>::updateElem(const size_t & idx, const T& elem)
{
    _elems[idx] = elem;

    if(idx > 1 && higherPriority(_elems[idx], _elems[parent(idx)]))
    {
        heapifyUp(idx);
    }
    else
    {
        heapifyDown(idx);
    }
}


template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    return _elems.empty();
}

template <class T, class Compare>
void heap<T, Compare>::getElems(std::vector<T> & heaped) const
{
    for (size_t i = root(); i < _elems.size(); i++) {
        heaped.push_back(_elems[i]);
    }
}
