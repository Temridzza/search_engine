#include "converterJSON.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <string>

#include "nlohmann/json.hpp"

nlohmann::json fillingJson(const std::string& nameFile) {
    std::ifstream file(nameFile);

    if (!file.is_open()) {
        throw std::runtime_error(nameFile + " file is missing");
    }

    nlohmann::json json;
    file >> json;

    file.close();
    return json;
}

std::string getTextFiles(const std::string& nameFile) {
    std::ifstream file(nameFile);
    std::string str;

    if (!file.is_open()) {
        std::cout << nameFile + " file is missing";
    }

    file >> str;

    file.close();
    return str;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {

    nlohmann::json configJson = fillingJson("../../files/config.json");

    if(!configJson.contains("config")) {
        throw std::invalid_argument("config file is empty");
    }

    std::vector<std::string> listFiles;

    auto it = configJson["files"].begin();
    while (it != configJson["files"].end()) {

        std::string fileData = getTextFiles(*it);

        listFiles.push_back(fileData);
        ++it;
    }

    return listFiles;
}

int ConverterJSON::GetResponsesLimit() {

    nlohmann::json responsesLimit = fillingJson("../../files/config.json");

    const int maxResponses = responsesLimit["config"]["max_responses"];

    return maxResponses;
}

std::vector<std::string> ConverterJSON::GetRequests() {

    nlohmann::json requestJson = fillingJson("../../files/requests.json");

    std::vector<std::string> listRequest;

    auto it = requestJson["requests"].begin();
    while (it != requestJson["requests"].end()) {
        listRequest.push_back(*it);
        ++it;
    }

    return listRequest;
}

std::string fillRequestId(const int& answer) {
    const int lengthRequest = (std::to_string(answer)).length();

    if(lengthRequest == 1) return "request00" + std::to_string(answer);

    if(lengthRequest == 2) return "request0" + std::to_string(answer);

    if(lengthRequest == 3) return "request" + std::to_string(answer);

    return "";
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>>& answers) {

    nlohmann::json json;

    json["answers"] = {};

    int idRequest = 0;
    for (const auto& answer : answers) {
        const std::string requestId = fillRequestId(idRequest);

        json["answers"][requestId] = {};

        json["answers"][requestId]["result"] = answer[0].second > 0;

        if(answer[0].second > 0) {
            std::vector<std::string> relevance;

            for (const auto&[fst, snd] : answer) {
                relevance.push_back("docid: " + std::to_string(fst));
                relevance.push_back("rank: " + std::to_string(snd));
            }

            json["answers"][requestId]["relevance"] = relevance;
        }
        idRequest++;
    }

    std::ofstream outFile("../../files/answers.json");

    if (!outFile.is_open()) {
        throw std::runtime_error(" file answers is missing");
    }
    outFile << json.dump(4);
    outFile.close();
}