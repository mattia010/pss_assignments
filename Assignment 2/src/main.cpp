#include <iostream>
#include <cstdlib>
#include <string>

#include "src/CollectionManager.hpp"

int main(int argc, char* argv[]) {
    std::system("clear");

    CollectionManager cm;
    cm.loadCollection();

    bool exit = false;
    bool wrong = false;
    int choice;
    while (!exit) {
        std::system("clear");
        choice = 0;
        if (!wrong) {
            std::cout << "Select your next operation (specify its index):" << std::endl;
        } else {
            std::cout << "Invalid operation index! Try again. (specify a valid operation index):" << std::endl;
            wrong = false;
        }
        std::cout << "1 - Aggiungi Dipinto" << std::endl;
        std::cout << "2 - Rimuovi Dipinto" << std::endl;
        std::cout << "3 - Cerca Dipinti" << std::endl;
        std::cout << "4 - Ricarica Collezione" << std::endl;
        std::cout << "5 - Elenca Dipinti" << std::endl;
        std::cout << "6 - Esci" << std::endl;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::system("clear");

                std::cout << "Inserisci la scuola:" << std::endl;
                std::string school;
                std::cin >> school;
                std::cout << "Inserisci l'autore:" << std::endl;
                std::string author;
                std::cin >> author;
                std::cout << "Inserisci il titolo:" << std::endl;
                std::string title;
                std::cin >> title;
                std::cout << "Inserisci l'anno:" << std::endl;
                std::string year;
                std::cin >> year;
                std::cout << "Inserisci la sala in cui si trova l'opera:" << std::endl;
                std::string room;
                std::cin >> room;

                std::system("clear");
                cm.addPainting(school, author, title, year, room);
                std::cout << "Inserimento completato." << std::endl;

                int stop;
                std::cin >> stop;
            }
            break;

            case 2: {
                std::system("clear");

                std::cout << "Specifica il titolo esatto del dipinto da rimuovere." << std::endl;
                std::string paintingTitle;
                std::cin >> paintingTitle;
                bool found = cm.removePainting(paintingTitle);

                std::system("clear");
                if (found) {
                    std::cout << "Dipinto rimosso con successo." << std::endl;
                } else {
                    std::cout << "Dipinto non trovato." << std::endl;
                }
                int stop;
                std::cin >> stop;
            }
            break;

            case 3: {
                std::system("clear");
                std::cout << "Inserisci filtro titolo:" << std::endl;
                std::string titleFilter;
                std::cin >> titleFilter;

                Set<Painting> showables = cm.searchPaintings(titleFilter);
                std::cout << "Galleria degli Uffizi - Collezione Dipinti filtrata" << std::endl;
                int index = 0;
                for (Set<Painting>::const_iterator it = showables.begin(); it != showables.end(); ++it) {
                    std::cout << index << " - " << it->title << std::endl;
                    ++index;
                }
                int stop;
                std::cin >> stop;
            }
            break;

            case 4: {
                cm.loadCollection();
            }
            break;

            case 5: {
                std::system("clear");

                std::cout << "Galleria degli Uffizi - Collezione Dipinti" << std::endl;
                for (unsigned int i = 0; i < cm.getCollection().size; ++i) {
                    std::cout << i << " - " << cm.getCollection()[i].title << std::endl;
                }
                int stop;
                std::cin >> stop;
            }
            break;

            case 6: {
                system("clear");
                exit = true;
            }
            break;

            default: {
                wrong = true;
            }
            break;
        }
    }

    return 0;
}

