//
// Created by kupri on 15.10.2024.
//

#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <vector>
#include <string>

class ConverterJSON {

public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> &answers);

};


#endif //SEARCH_ENGINE_CONVERTERJSON_H
