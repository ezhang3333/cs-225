/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() 
{ 
    length_ = 0;
    head_ = NULL;
    tail_ = NULL;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const 
{
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const 
{
  return List<T>::ListIterator(NULL);
}

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy()
 {
  ListNode* current = head_;

  while(current != NULL)
  {
    ListNode* next = current->next;
    delete(current);
    current = next;
  }

  head_ = NULL;
  tail_ = NULL;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) 
{
  ListNode * newNode = new ListNode(ndata);

  if(head_ == NULL)
  {
    head_ = newNode;
    tail_ = newNode;
    newNode->next = NULL;
    newNode->prev = NULL;
  }
  else
  {
    newNode->next = head_;
    newNode->prev = NULL;
    head_->prev = newNode;
    head_ = newNode;
  }

  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) 
{
  ListNode* newNode = new ListNode(ndata);

  if(tail_ == NULL)
  {
    head_ = newNode;
    tail_ = newNode;
    tail_->next = NULL;
    tail_->prev = NULL;
  }
  else
  {
    tail_->next = newNode;
    newNode->next = NULL;
    newNode->prev = tail_;
    tail_ = newNode;
  }

  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode* List<T>::split(ListNode* start, int splitPoint) 
{
  ListNode* curr = start;

  for(int i = 0; i < splitPoint && curr != NULL; i++)
  {
    curr = curr->next;
  }

  if(curr == NULL) return NULL;

  if(curr->prev != NULL) curr->prev->next = NULL;
  
  curr->prev = NULL;

  return curr;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() 
{
  if(head_ == NULL || head_->next == NULL)
  {
    return;
  }


  ListNode* curr = head_;
  
  while(curr != NULL && curr->next != tail_ && curr->next != NULL)
  {
    ListNode* temp = curr->next;
    curr->next = temp->next;
    if(temp->next != NULL)
    {
      temp->next->prev = curr;
    }

    temp->prev = tail_;
    tail_->next = temp;
    tail_ = temp;
    tail_->next = NULL;  
  
    curr = curr->next;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() 
{
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) 
{
  if(startPoint == NULL || endPoint == NULL || startPoint == endPoint)
  {
    return;
  }

  ListNode* startPrev = startPoint->prev;
  ListNode* endNext = endPoint->next;

  ListNode* curr = startPoint;
  ListNode* temp = NULL;


  while(curr != endNext)
  {
    temp = curr->next;
    curr->next = curr->prev;
    curr->prev = temp;

    curr = curr->prev;
  }

  if (startPrev != NULL)
  {
    startPrev->next = endPoint;
  } 
  else 
  {
    head_ = endPoint;  
  }

  if (endNext != NULL)
  {
      endNext->prev = startPoint;
  } 
  else 
  {
      tail_ = startPoint;  
  }

  startPoint = endPoint;
  endPoint = startPrev != NULL ? startPrev->next : head_;
  
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) 
{
    // If n is greater than or equal to the list length, reverse the entire list
    if (n >= length_) {
        reverse(head_, tail_);
        return;
    }

    // No need to reverse if n <= 1
    if (n <= 1) {
        return;
    }

    ListNode* blockStart = head_; // Start of the current block
    ListNode* blockEnd = nullptr; // End of the current block
    
    // Iterate through the list in blocks of size `n`
    while (blockStart != nullptr) {
        // Move `blockEnd` to the end of the current block (n-1 steps ahead of blockStart)
        blockEnd = blockStart;
        for (int i = 1; i < n && blockEnd->next != nullptr; ++i) {
            blockEnd = blockEnd->next;
        }

        // Reverse the current block
        reverse(blockStart, blockEnd);

        // Move blockStart to the next block, which is `blockEnd->next`
        // After reverse, blockStart will be the original blockEnd, so we need blockEnd->next
        blockStart = blockEnd->next;
    }
}



/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) 
{
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode* List<T>::merge(ListNode* first, ListNode* second) {
    // Handle the case when one or both lists are empty
    if (!first) return second;
    if (!second) return first;

    // Pointer to keep track of the merged list's head
    ListNode* mergedHead = nullptr;

    // Initialize the merged list with the smaller head
    if (first->data < second->data) {
        mergedHead = first;
        first = first->next;
    } else {
        mergedHead = second;
        second = second->next;
    }

    // Pointer to keep track of the last node in the merged list
    ListNode* current = mergedHead;

    // Merge the remaining nodes in sorted order
    while (first && second) 
    {
        if (first->data < second->data) 
        {
            current->next = first;
            first = first->next;
        } 
        else 
        {
            current->next = second;
            second = second->next;
        }
        current = current->next;
    }

    // Append the remaining nodes from either list
    if (first) {
        current->next = first;
    } else {
        current->next = second;
    }

    return mergedHead;
}


/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
    // Base case: If the list is empty or has only one element, it's already sorted.
    if (chainLength <= 1) {
        return start;
    }

    // Step 1: Split the list into two halves using the split function
    ListNode* secondHalf = split(start, chainLength / 2);

    // Step 2: Recursively sort both halves
    ListNode* left = mergesort(start, chainLength / 2);
    ListNode* right = mergesort(secondHalf, chainLength - chainLength / 2);

    // Step 3: Merge the sorted halves
    return merge(left, right);
}

