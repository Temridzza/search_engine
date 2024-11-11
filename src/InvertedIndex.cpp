#include "InvertedIndex.h"
#include "converterJSON.h"

#include <thread>
#include <mutex>

std::mutex mtx;

std::vector<std::string> splitIntoWords(const std::string& str){
    std::vector<std::string> wordList;
    std::string word;
    for(int i = 0; i < str.length(); i++) {
        if (str[i] == ' ')
        {
            wordList.push_back(word);
            word.clear();
        } else {
            word += str[i];
        }
    }
    wordList.push_back(word);

    return wordList;
}

void fillingFreq_dictionary(const std::string& str, const size_t& current_doc_id,
                            std::map<std::string, std::vector<Entry>>* freq_dictionary) {

    std::vector<std::string> words = splitIntoWords(str);

    for (auto word : words) {

        if (auto it = freq_dictionary->find(word); it != freq_dictionary->end()) {
            if (!it->second.empty()) {
                bool doc_id_found = false;

                for (auto &[doc_id, count] : it->second) {
                    if (doc_id == current_doc_id) {
                        std::lock_guard lock(mtx);
                        count++;
                        doc_id_found = true;
                        break;
                    }
                }

                if (!doc_id_found) {
                    std::lock_guard lock(mtx);
                    it->second.push_back({current_doc_id, 1});
                }
            } else {
                std::lock_guard lock(mtx);
                freq_dictionary->operator[](word).push_back({current_doc_id, 1});
            }
        } else {
            std::lock_guard lock(mtx);
            (*freq_dictionary)[word].push_back({current_doc_id, 1});
        }
        word.clear();
    }
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {

    docs.clear();
    for(const auto& input : input_docs)
        docs.push_back(input);

    freq_dictionary.clear();

    std::vector<std::thread> threads;
    threads.reserve(docs.size());

    for(size_t i = 0; i < docs.size(); i++)
        threads.emplace_back(fillingFreq_dictionary, docs[i], i, &freq_dictionary);

    for (auto& thread : threads)
        thread.join();
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (const auto it = freq_dictionary.find(word); it != freq_dictionary.end())
        return it->second;

    return {};
}