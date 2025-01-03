#include <vector>
#include <string>

#include "src/CsvParser.hpp"

Set<Painting> CsvParser::readCsv(std::string filepath) {
    std::ifstream in(filepath, std::ios_base::in);
    std::string line;
    std::istringstream is;
    Set<Painting> collection;
    char c;
    bool escaping;
    std::vector<std::string> stringVec;
    std::string tmpStr;
    while (std::getline(in, line)) {
        is = std::istringstream(line);
        /*std::getline(is, schoolStr, ',');
        std::getline(is, nameStr, ',');
        std::getline(is, authorStr, ',');
        std::getline(is, yearStr, ',');
        std::getline(is, roomStr, ',');*/

        escaping = false;
        stringVec.clear();
        while (is >> std::noskipws >> c) {
            if (c == '\"') {
                escaping = !escaping;
                continue;
            }
            if (c == ',' && !escaping) {
                stringVec.push_back(tmpStr);
                tmpStr.clear();
                continue;
            }
            tmpStr.push_back(c);
        }
        if (tmpStr != "") {
            stringVec.push_back(tmpStr);
            tmpStr.clear();
        }
        if (stringVec.size() != 5) {
            throw std::invalid_argument(std::to_string(stringVec.size()));
        }
        collection.add(Painting(stringVec.at(0), stringVec.at(1), stringVec.at(2), stringVec.at(3), stringVec.at(4)));
    }
    return collection;
}

