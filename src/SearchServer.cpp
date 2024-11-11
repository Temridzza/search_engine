#include "SearchServer.h"
#include "converterJSON.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <utility>



std::unordered_map<std::string, int> dictionaryFormation(const std::vector<std::string>& wordList) {
    std::unordered_map<std::string, int> result;

    for (const auto& w : wordList) {
        ++result[w];
    }
    return result;
}

std::vector<std::pair<std::string, int>> dictionarySort(const std::unordered_map<std::string, int>& freq_dict) {
    std::vector<std::pair<std::string, int>> result;
    result.reserve(freq_dict.size());

    for (const auto& p : freq_dict) {
        result.emplace_back(p);
    }
    std::sort(result.begin(), result.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    return result;
}

std::unordered_map<std::string, std::vector<Entry>> searchDocId(InvertedIndex& index,
        const std::vector<std::pair<std::string, int>>& sorted_freq) {

    std::unordered_map<std::string, std::vector<Entry>> result;

    for (const auto&[fst, snd] : sorted_freq) {
        if(const auto entry = index.GetWordCount(fst); !entry.empty())
            result[fst] = entry;
    }
    return result;
}

std::map<size_t, float> calculationAbsoluteRelevance(const std::unordered_map<std::string, std::vector<Entry>>& currentDocId) {

    std::map<size_t, float> result;

    for (const auto& [frs, snd] : currentDocId) {
        std::vector<Entry> entry = snd;

        for(const auto&[doc_id, count] : entry)
            result[doc_id] += count;
    }
    return result;
}

float searchMaxAbsoluteRelevance(const std::map<size_t, float>& absoluteRelevance) {
    float result = 0;

    for (const auto&[fst, snd] : absoluteRelevance) {
        if (snd > result)
            result = snd;
    }
    return result;
}

void bubbleSortDescending(std::vector<RelativeIndex>* vec) {
    size_t n = vec->size();
    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if ((*vec)[j].rank < (*vec)[j + 1].rank) {
                std::swap((*vec)[j], (*vec)[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void calculationRelativeRelevance(const std::map<size_t, float>& absoluteRelevance, const float& maxAbsoluteRelevance,
                                  std::vector<std::vector<RelativeIndex>>* result) {

    ConverterJSON converter;
    const int responsesLimit = converter.GetResponsesLimit();

    std::vector<RelativeIndex> currentRelative;
    float rank = 0;
    for (auto [fst, snd] : absoluteRelevance) {
        rank = snd / maxAbsoluteRelevance;
        currentRelative.push_back({fst, rank});
    }

    bubbleSortDescending(&currentRelative);

    std::vector<RelativeIndex> resultRelative;
    for(int i = 0; currentRelative.size() > i && resultRelative.size() < responsesLimit; i++) {
        resultRelative.push_back(currentRelative[i]);
    }

    result->push_back(resultRelative);
}

void writingFile(const std::vector<std::vector<RelativeIndex>>& result) {
    std::vector<std::vector<std::pair<int, float>>> converted;
    std::vector<std::pair<int, float>> docs;

    for(const auto& doc : result) {
        if(!doc.empty())
            for(auto [doc_id, rank] : doc) {
                docs.emplace_back(doc_id, rank);
            }
        else {
            docs.emplace_back();
        }
        converted.emplace_back(docs);
        docs.clear();
    }
    ConverterJSON obj;
    obj.putAnswers(converted);
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result;

    for(const auto& query : queries_input) {

        std::vector<std::string> listWords = splitIntoWords(query);
        std::unordered_map<std::string, int> freq_dict = dictionaryFormation(listWords);
        std::vector<std::pair<std::string, int>> sorted_freq = dictionarySort(freq_dict);
        std::unordered_map<std::string, std::vector<Entry>> currentDocId = searchDocId(_index, sorted_freq);

        if(currentDocId.empty()) {
            result.emplace_back();
            break;
        }

        std::map<size_t, float> absoluteRelevance = calculationAbsoluteRelevance(currentDocId);
        float maxAbsoluteRelevance = searchMaxAbsoluteRelevance(absoluteRelevance);

        calculationRelativeRelevance(absoluteRelevance, maxAbsoluteRelevance, &result);
    }

    writingFile(result);

    return result;
}