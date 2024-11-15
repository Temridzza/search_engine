#include <iostream>
#include "converterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    try {
        ConverterJSON converterJSON;
        const std::vector<std::string> vecTextDocument = converterJSON.GetTextDocuments();

        InvertedIndex index;
        index.UpdateDocumentBase(vecTextDocument);

        const std::vector<std::string> vecRequests = converterJSON.GetRequests();

        SearchServer srv(index);
        srv.search(vecRequests);

    }catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}