#ifndef SRC_PAINTING_HPP_
#define SRC_PAINTING_HPP_

#include <string>

struct Painting {
    std::string school;
    std::string author;
    std::string title;
    std::string year;
    std::string room;

    Painting() : school(""), author(""), title(""), year(""), room("") {}

    Painting(std::string schoolParam,
            std::string authorParam,
            std::string titleParam,
            std::string yearParam,
            std::string roomParam) :
        school(schoolParam),
        author(authorParam),
        title(titleParam),
        year(yearParam),
        room(roomParam) {}

    bool operator==(const Painting &other) {
        if (school != other.school
            || author != other.author
            || title != other.title
            || year != other.year
            || room != other.room) {
            return false;
        }
        return true;
    }
};

#endif  // SRC_PAINTING_HPP_
