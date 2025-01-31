/**
 * @file lphashtable.cpp
 * Implementation of the LPHashTable class.
 */
#include "lphashtable.h"

template <class K, class V>
LPHashTable<K, V>::LPHashTable(size_t tsize)
{
    if (tsize <= 0)
        tsize = 17;
    size = findPrime(tsize);
    table = new std::pair<K, V>*[size];
    should_probe = new bool[size];
    for (size_t i = 0; i < size; i++) {
        table[i] = NULL;
        should_probe[i] = false;
    }
    elems = 0;
}

template <class K, class V>
LPHashTable<K, V>::~LPHashTable()
{
    for (size_t i = 0; i < size; i++)
        delete table[i];
    delete[] table;
    delete[] should_probe;
}

template <class K, class V>
LPHashTable<K, V> const& LPHashTable<K, V>::operator=(LPHashTable const& rhs)
{
    if (this != &rhs) {
        for (size_t i = 0; i < size; i++)
            delete table[i];
        delete[] table;
        delete[] should_probe;

        table = new std::pair<K, V>*[rhs.size];
        should_probe = new bool[rhs.size];
        for (size_t i = 0; i < rhs.size; i++) {
            should_probe[i] = rhs.should_probe[i];
            if (rhs.table[i] == NULL)
                table[i] = NULL;
            else
                table[i] = new std::pair<K, V>(*(rhs.table[i]));
        }
        size = rhs.size;
        elems = rhs.elems;
    }
    return *this;
}

template <class K, class V>
LPHashTable<K, V>::LPHashTable(LPHashTable<K, V> const& other)
{
    table = new std::pair<K, V>*[other.size];
    should_probe = new bool[other.size];
    for (size_t i = 0; i < other.size; i++) {
        should_probe[i] = other.should_probe[i];
        if (other.table[i] == NULL)
            table[i] = NULL;
        else
            table[i] = new std::pair<K, V>(*(other.table[i]));
    }
    size = other.size;
    elems = other.elems;
}

template <class K, class V>
void LPHashTable<K, V>::insert(K const& key, V const& value)
{
    elems++;  // Increment the element count.
    
    // Check if the table needs resizing.
    if (static_cast<double>(elems) / size >= 0.7) {
        resizeTable();
    }

    // Compute the initial hash index.
    size_t idx = HashTable<K, V>::hash(key, size);
    
    // Linear probing to find an empty slot.
    while (table[idx] != NULL) {
        idx = (idx + 1) % size;
    }

    // Insert the new pair.
    table[idx] = new std::pair<K, V>(key, value);
    should_probe[idx] = true;  // Mark the slot as probed.
}

template <class K, class V>
void LPHashTable<K, V>::remove(K const& key)
{
    int idx = findIndex(key);
    if (idx != -1) {
        delete table[idx];
        table[idx] = NULL;
        elems--;  // Decrement the element count.
    }
}

template <class K, class V>
int LPHashTable<K, V>::findIndex(const K& key) const
{
    size_t idx = HashTable<K, V>::hash(key, size);
    size_t start = idx;

    // Linear probing to find the key.
    while (should_probe[idx]) {
        if (table[idx] != NULL && table[idx]->first == key) {
            return static_cast<int>(idx);
        }
        idx = (idx + 1) % size;

        // Stop if we've wrapped around the table.
        if (idx == start) {
            break;
        }
    }
    return -1;  // Key not found.
}

template <class K, class V>
V LPHashTable<K, V>::find(K const& key) const
{
    int idx = findIndex(key);
    if (idx != -1)
        return table[idx]->second;
    return V();
}

template <class K, class V>
V& LPHashTable<K, V>::operator[](K const& key)
{
    // First, attempt to find the key and return its value by reference
    int idx = findIndex(key);
    if (idx == -1) {
        // otherwise, insert the default value and return it
        insert(key, V());
        idx = findIndex(key);
    }
    return table[idx]->second;
}

template <class K, class V>
bool LPHashTable<K, V>::keyExists(K const& key) const
{
    return findIndex(key) != -1;
}

template <class K, class V>
void LPHashTable<K, V>::clear()
{
    for (size_t i = 0; i < size; i++)
        delete table[i];
    delete[] table;
    delete[] should_probe;
    table = new std::pair<K, V>*[17];
    should_probe = new bool[17];
    for (size_t i = 0; i < 17; i++)
        should_probe[i] = false;
    size = 17;
    elems = 0;
}

template <class K, class V>
void LPHashTable<K, V>::resizeTable()
{
    size_t newSize = findPrime(2 * size);

    // Allocate new storage.
    std::pair<K, V>** newTable = new std::pair<K, V>*[newSize];
    bool* newShouldProbe = new bool[newSize];

    for (size_t i = 0; i < newSize; i++) {
        newTable[i] = NULL;
        newShouldProbe[i] = false;
    }

    // Rehash all elements.
    for (size_t i = 0; i < size; i++) {
        if (table[i] != NULL) {
            size_t idx = HashTable<K, V>::hash(table[i]->first, newSize);
            while (newTable[idx] != NULL) {
                idx = (idx + 1) % newSize;
            }
            newTable[idx] = table[i];
            newShouldProbe[idx] = true;
        }
    }

    // Delete old table.
    delete[] table;
    delete[] should_probe;

    table = newTable;
    should_probe = newShouldProbe;
    size = newSize;
}
