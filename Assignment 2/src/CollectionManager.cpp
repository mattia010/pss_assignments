#include <string>

#include "src/CollectionManager.hpp"

void CollectionManager::addPainting(std::string school, std::string author, std::string title, std::string year, std::string room) {
    Painting p{school, author, title, year, room};
    collection.add(p);
}

bool CollectionManager::removePainting(std::string title) {
    bool contains = false;
    for (Set<Painting>::const_iterator it = collection.begin(); it != collection.end(); ++it) {
        if (it->title == title) {
            collection.remove(*it);
            contains = true;
        }
    }
    return contains;
}

Set<Painting> CollectionManager::searchPaintings(std::string titleFilter) {
    Set<Painting> res;
    bool schoolTrue, authorTrue, titleTrue, yearTrue, roomTrue;
    for (Set<Painting>::const_iterator it = collection.begin(); it != collection.end(); ++it) {
        if (it->title.find(titleFilter) == std::string::npos) {
            continue;
        }
        res.add(*it);
    }
    return res;
}

void CollectionManager::loadCollection() {
    collection = CsvParser::readCsv("db/dipinti_uffizi.csv");
}

Set<Painting> CollectionManager::getCollection() {
    return collection;
}
