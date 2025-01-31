/**
 * @file schashtable.cpp
 * Implementation of the SCHashTable class.
 */

#include "schashtable.h"
 
template <class K, class V>
SCHashTable<K, V>::SCHashTable(size_t tsize)
{
    if (tsize <= 0)
        tsize = 17;
    size = findPrime(tsize);
    table = new std::list<std::pair<K, V>>[size];
    elems = 0;
}

template <class K, class V>
SCHashTable<K, V>::~SCHashTable()
{
    delete[] table;
}

template <class K, class V>
SCHashTable<K, V> const& SCHashTable<K, V>::
operator=(SCHashTable<K, V> const& rhs)
{
    if (this != &rhs) {
        delete[] table;
        table = new std::list<std::pair<K, V>>[rhs.size];
        for (size_t i = 0; i < rhs.size; i++)
            table[i] = rhs.table[i];
        size = rhs.size;
        elems = rhs.elems;
    }
    return *this;
}

template <class K, class V>
SCHashTable<K, V>::SCHashTable(SCHashTable<K, V> const& other)
{
    table = new std::list<std::pair<K, V>>[other.size];
    for (size_t i = 0; i < other.size; i++)
        table[i] = other.table[i];
    size = other.size;
    elems = other.elems;
}

template <class K, class V>
void SCHashTable<K, V>::insert(K const& key, V const& value)
{
    size_t idx = hashes::hash(key, size); // Compute hash index
    table[idx].push_front(std::make_pair(key, value)); // Insert the key-value pair
    ++elems; // Increment element count

    if (shouldResize()) // Check if resizing is needed
        resizeTable();
}

template <class K, class V>
void SCHashTable<K, V>::remove(K const& key)
{
    size_t idx = hashes::hash(key, size); // Compute hash index
    typename std::list<std::pair<K, V>>::iterator it;

    for (it = table[idx].begin(); it != table[idx].end(); ++it) {
        if (it->first == key) { // Key found
            table[idx].erase(it); // Remove the key-value pair
            --elems; // Decrement element count
            return;
        }
    }
}

template <class K, class V>
V SCHashTable<K, V>::find(K const& key) const
{

    size_t idx = hashes::hash(key, size); // Compute hash index
    typename std::list<std::pair<K, V>>::const_iterator it;

    for (it = table[idx].begin(); it != table[idx].end(); ++it) {
        if (it->first == key) // Key found
            return it->second;
    }
}

template <class K, class V>
V& SCHashTable<K, V>::operator[](K const& key)
{
    size_t idx = hashes::hash(key, size);
    typename std::list<std::pair<K, V>>::iterator it;
    for (it = table[idx].begin(); it != table[idx].end(); it++) {
        if (it->first == key)
            return it->second;
    }

    // was not found, insert a default-constructed version and return it
    ++elems;
    if (shouldResize())
        resizeTable();

    idx = hashes::hash(key, size);
    std::pair<K, V> p(key, V());
    table[idx].push_front(p);
    return table[idx].front().second;
}

template <class K, class V>
bool SCHashTable<K, V>::keyExists(K const& key) const
{
    size_t idx = hashes::hash(key, size);
    typename std::list<std::pair<K, V>>::iterator it;
    for (it = table[idx].begin(); it != table[idx].end(); it++) {
        if (it->first == key)
            return true;
    }
    return false;
}

template <class K, class V>
void SCHashTable<K, V>::clear()
{
    delete[] table;
    table = new std::list<std::pair<K, V>>[17];
    size = 17;
    elems = 0;
}

template <class K, class V>
void SCHashTable<K, V>::resizeTable()
{
    size_t newSize = findPrime(size * 2); // Find next prime size
    std::list<std::pair<K, V>>* newTable = new std::list<std::pair<K, V>>[newSize];

    // Rehash all elements into the new table
    for (size_t i = 0; i < size; i++) {
        typename std::list<std::pair<K, V>>::iterator it;
        for (it = table[i].begin(); it != table[i].end(); ++it) {
            size_t newIdx = hashes::hash(it->first, newSize); // Recompute hash
            newTable[newIdx].push_front(*it);
        }
    }

    delete[] table; // Delete old table
    table = newTable; // Point to new table
    size = newSize; // Update size
}
