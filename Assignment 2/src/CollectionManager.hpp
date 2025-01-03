#ifndef SRC_COLLECTIONMANAGER_HPP_
#define SRC_COLLECTIONMANAGER_HPP_

#include <string>

#include "src/Set.hpp"
#include "src/Painting.hpp"
#include "src/CsvParser.hpp"

class CollectionManager {
 public:
    void addPainting(std::string school, std::string author, std::string title, std::string year, std::string room);
    bool removePainting(std::string title);
    Set<Painting> searchPaintings(std::string titleFilter);
    void loadCollection();
    Set<Painting> getCollection();
 private:
    Set<Painting> collection;
};

#endif  // SRC_COLLECTIONMANAGER_HPP_
