/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    Node* result = find(root, key);

    if(result == nullptr)
    {
        return V{};
    }
    
    return result->value;
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    if(subtree == nullptr)
    {
        return subtree;
    }

    if(key == subtree->key)
    {
        return subtree;
    }
    else if(key > subtree->key)
    {
        return find(subtree->right, key);
    }
    else
    {
        return find(subtree->left, key);
    }
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    Node*& insertNode = find(subtree, key);

    if(insertNode == nullptr)
    {
        insertNode = new Node(key, value);
    }
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    K tempKey = first->key;
    V tempVal = first->value;

    first->key = second->key;
    first->value = second->value;
    second->key = tempKey;
    second->value = tempVal;
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    if(subtree == nullptr)
    {
        return;
    }

    Node*& removeNode = find(subtree, key);

    if(removeNode == nullptr)
    {
        return;
    }
    else
    {
        if(removeNode->left == nullptr && removeNode->right == nullptr)
        {
            delete removeNode;
            removeNode = nullptr;
            return;
        }
        else if(removeNode->left == nullptr)
        {
            Node* temp = removeNode;
            removeNode = removeNode->right;
            delete temp;
            return;
        }
        else if(removeNode->right == nullptr)
        {
            Node* temp = removeNode;
            removeNode = removeNode->right;
            delete temp;
            return;
        }
        else
        {
            Node* inOrderPredecessor = removeNode->left;

            while(inOrderPredecessor->right != nullptr)
            {
                inOrderPredecessor = inOrderPredecessor->right;
            }

            K temp = inOrderPredecessor->key;
            swap(removeNode, inOrderPredecessor);
            remove(removeNode->left, temp);
            return;
        }
    }




    /*
    if(subtree == nullptr)
    {
        return;
    }

    if(subtree->key > key)
    {
        remove(subtree->left, key);
    }
    else if(subtree->key < key)
    {
        remove(subtree->right, key);
    }
    else 
    {
        // Step 2: Found the node to be removed
        if (subtree->left == nullptr && subtree->right == nullptr) 
        {
            // Case 1: Node is a leaf (no children)
            delete subtree;
            subtree = nullptr;
        } 
        else if (subtree->left == nullptr) 
        {
            // Case 2: Node has only right child
            Node* temp = subtree;
            subtree = subtree->right;
            delete temp;
        } 
        else if (subtree->right == nullptr) 
        {
            // Case 3: Node has only left child
            Node* temp = subtree;
            subtree = subtree->left;
            delete temp;
        } 
        else 
        {
            // Case 4: Node has two children
            // Find the in-order successor (smallest in the right subtree)
            Node* inOrder = subtree->right;
            while (inOrder->left != nullptr)
             {
                inOrder = inOrder->left;
            }

            // Replace the node's key and value with the successor's key and value
            subtree->key = inOrder->key;
            subtree->value = inOrder->value;

            // Remove the in-order successor
            remove(subtree->right, inOrder->key);
        }
    }
    */
}



template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList)
{
    BST<K,V> bst;

    for(int i = 0; i < (int)inList.size(); i++)
    {
        bst.insert(inList[i].first , inList[i].second);
    }

    return bst;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList)
{
    std::vector<int> heightHistogram; // To store the count of trees by height

    // Sort the input list to start generating permutations from the first lexicographical order
    std::sort(inList.begin(), inList.end());

    do {
        // Build a BST using the current permutation
        BST<K, V> bst = listBuild(inList);
        
        // Calculate the height of the BST
        int height = bst.height(); // Directly use the height function from the BST class

        // Ensure the histogram has enough space to record this height
        if ((int)height >= (int)heightHistogram.size()) {
            heightHistogram.resize(height + 1); // Expand the vector if needed
        }

        // Increment the count of trees with this height
        heightHistogram[height]++;

    } while (std::next_permutation(inList.begin(), inList.end())); // Generate next permutation

    return heightHistogram;
}