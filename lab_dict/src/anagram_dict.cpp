/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> 
#include <fstream>
#include <iterator>

using std::string;
using std::vector;
using std::ifstream;
using std::map;

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        return; // If the file can't be opened, leave dict empty
    }

    string word;
    while (std::getline(file, word)) {
        string sorted_word = word;
        std::sort(sorted_word.begin(), sorted_word.end());
        dict[sorted_word].push_back(word);
    }
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    for (const auto& word : words) {
        string sorted_word = word;
        std::sort(sorted_word.begin(), sorted_word.end());
        dict[sorted_word].push_back(word);
    }
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    string sorted_word = word;
    std::sort(sorted_word.begin(), sorted_word.end());

    if (dict.find(sorted_word) != dict.end()) {
        return dict.at(sorted_word);
    }
    return {};
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are omitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    vector<vector<string>> result;

    for (const auto& pair : dict) {
        if (pair.second.size() > 1) { // Only include groups with more than one word
            result.push_back(pair.second);
        }
    }

    return result;
}
