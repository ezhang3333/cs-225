/**
 * @file common_words.cpp
 * Implementation of the CommonWords class.
 *
 * @author Zach Widder
 * @date Fall 2014
 */

#include "common_words.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <unordered_set>

using std::string;
using std::vector;
using std::ifstream;
using std::map;
using std::cout;
using std::endl;

string remove_punct(const string& str)
{
    string ret;
    std::remove_copy_if(str.begin(), str.end(), std::back_inserter(ret),
                        [](int c) {return std::ispunct(c);});
    return ret;
}

CommonWords::CommonWords(const vector<string>& filenames)
{
    // Initialize all member variables
    init_file_word_maps(filenames);
    init_common();
}

void CommonWords::init_file_word_maps(const vector<string>& filenames)
{
    // Resize file_word_maps to match the number of filenames
    file_word_maps.resize(filenames.size());

    for (size_t i = 0; i < filenames.size(); ++i) {
        vector<string> words = file_to_vector(filenames[i]);

        for (const auto& word : words) {
            file_word_maps[i][word]++;
        }
    }
}

void CommonWords::init_common()
{
    // Traverse each file's word map
    for (const auto& file_map : file_word_maps) {
        for (const auto& entry : file_map) {
            const string& word = entry.first;
            common[word]++;
        }
    }
}

/**
 * @param n The number of times a word has to appear in each file.
 * @return A vector of strings. The vector contains all words that appear
 * in each file >= n times.
 */
vector<string> CommonWords::get_common_words(unsigned int n) const
{
    vector<string> out;

    // Check each word in `common` map
    for (const auto& entry : common) {
        const string& word = entry.first;
        unsigned int doc_count = entry.second;

        if (doc_count == file_word_maps.size()) {  // Appears in all files
            bool appears_n_times = true;

            // Check if the word appears >= n times in each file
            for (const auto& file_map : file_word_maps) {
                if (file_map.find(word) == file_map.end() || file_map.at(word) < n) {
                    appears_n_times = false;
                    break;
                }
            }

            if (appears_n_times) {
                out.push_back(word);
            }
        }
    }

    return out;
}

/**
 * Takes a filename and transforms it to a vector of all words in that file.
 * @param filename The name of the file that will fill the vector
 */
vector<string> CommonWords::file_to_vector(const string& filename) const
{
    ifstream words(filename);
    vector<string> out;

    if (words.is_open()) {
        std::istream_iterator<string> word_iter(words);
        std::istream_iterator<string> end;
        while (word_iter != end) {
            out.push_back(remove_punct(*word_iter));
            ++word_iter;
        }
    }
    return out;
}
