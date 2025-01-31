/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <tuple>
#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                                                         const string &word_list_fname)
{
    vector<std::tuple<std::string, std::string, std::string>> ret;

    // Read the word list
    ifstream wordsFile(word_list_fname);
    if (!wordsFile.is_open()) {
        return ret; // Return empty vector if the file cannot be opened
    }

    string word;
    while (getline(wordsFile, word)) {
        if (word.length() < 2) {
            continue; // Skip words that are too short
        }

        // Generate two subwords
        string word1 = word.substr(1);             // Remove the first letter
        string word2 = word[0] + word.substr(2);  // Remove the second letter

        // Check if the word, word1, and word2 are homophones
        if (d.homophones(word, word1) && d.homophones(word, word2)) {
            ret.emplace_back(word, word1, word2);
        }
    }

    return ret;
}

