#ifndef SRC_CSVPARSER_HPP_
#define SRC_CSVPARSER_HPP_

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "src/Set.hpp"
#include "src/Painting.hpp"

class CsvParser {
 public:
    static Set<Painting> readCsv(std::string filepath);
};

#endif  // SRC_CSVPARSER_HPP_
