#include "bloom.h"
#include <vector>


BF::BF(uint64_t size, std::vector<hashFunction> hashList) 
    : hv(std::move(hashList)), bv(size, false) {}

BF::BF(const BF& other) {
    // If hv is a pointer, deep copy the object it points to
    if (other.hv) {
        hv = new SomeType(*other.hv); // Deep copy the object pointed to by hv
    } else {
        hv = nullptr;
    }

    // If bv is a pointer, deep copy the vector it points to
    if (other.bv) {
        bv = new std::vector<int>(*other.bv); // Deep copy the vector
    } else {
        bv = nullptr;
    }
}


// Destructor
BF::~BF() = default; // No dynamic memory allocation to clean up.

// Add a key to the bloom filter
void BF::add(const int& key) {
    for (const auto& hash : hv) {
        int index = hash(key) % bv.size();
        bv[index] = true; // Mark the corresponding bit as true.
    }
}

// Check if a key is in the bloom filter
bool BF::contains(const int& key) const {
    for (const auto& hash : hv) {
        int index = hash(key) % bv.size();
        if (!bv[index]) {
            return false; // If any bit is unset, the key is not present.
        }
    }
    return true; // All relevant bits are set.
}

// Perform bitwise union of two bloom filters
void BF::bit_union(const BF& other) {
    for (size_t i = 0; i < bv.size(); ++i) {
        bv[i] = bv[i] || other.bv[i];
    }
}

// Perform bitwise intersection of two bloom filters
void BF::intersect(const BF& other) {
    for (size_t i = 0; i < bv.size(); ++i) {
        bv[i] = bv[i] && other.bv[i];
    }
}

// Measure the False Positive Rate (FPR)
float measureFPR(std::vector<int> inList, uint64_t size, std::vector<hashFunction> hashList, unsigned max) {
    BF bloom(size, hashList);

    // Add all elements from inList to the bloom filter.
    for (const auto& value : inList) {
        bloom.add(value);
    }

    unsigned falsePositives = 0;
    unsigned trueNegatives = 0;

    // Iterate through the entire universe (0 to max-1).
    for (unsigned i = 0; i < max; ++i) {
        if (std::find(inList.begin(), inList.end(), i) != inList.end()) {
            // Skip true positives as we're only interested in FP and TN.
            continue;
        }
        if (bloom.contains(i)) {
            ++falsePositives; // False positive detected.
        } else {
            ++trueNegatives; // Correctly identified as not present.
        }
    }

    // Calculate FPR = FP / (FP + TN).
    return static_cast<float>(falsePositives) / (falsePositives + trueNegatives);
}

// Get the bit value at a specific index in a byte
bool getBitFromByte(char in, int index) {
    // Cast `in` to unsigned to avoid sign extension issues
    return (static_cast<unsigned char>(in) & (1 << (7 - index))) != 0;
}

// Get the bit value at a specific index in a vector of bytes
bool getBitFromArray(std::vector<char> bv, int index) {
    int byteIndex = index / 8;  // Identify the byte.
    int bitIndex = index % 8;   // Identify the bit within the byte.
    return getBitFromByte(bv[byteIndex], bitIndex);
}

