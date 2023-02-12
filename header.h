#include <iostream>
#include <fstream>
#include <deque>
#include "json.hpp"

#ifndef HEADER_H_
#define HEADER_H_

using namespace std;
using json = nlohmann::json;

class BookInformation {
public:
    // Class constructor
    BookInformation(string input) {
        inputTitle = input;
        setAllDeques();
        setAllBookValues();
    }

    // Getter for ISBN
    string getISBN() {
        return ISBN;
    }

    // Getter for Title
    string getTitle() {
        return bookTitle;
    }

    // Getter for Author
    string getAuthor() {
        return bookAuthor;
    }

    // Getter for Year of Publication
    string getYearOfPub() {
        return yearOfPub;
    }

    string getPublisher() {
        return bookPublisher;
    }

    void Display();


private:
    string inputTitle;
    deque<string> isbns;
    deque<string> titles;
    deque<string> authors;
    deque<string> years;
    deque<string> publishers;
    string ISBN;
    string bookTitle;
    string bookAuthor;
    string yearOfPub;
    string bookPublisher;

    // Setter for all deques
    void setAllDeques();

    // Finds index number of title or ISBN if it exists
    int findIndexNum();

    // Setters for ISBN, Title, Author, Year of Publication, Publisher
    void setAllBookValues();
};

    void BookInformation::Display() {
        if (getTitle() == "Not found") {
            cout << "Invalid entry or book not found." << endl;
        }
        else {
            cout << endl;
            cout << "ISBN: " << getISBN() << endl;
            cout << "Title: " << getTitle() << endl;
            cout << "Author: " << getAuthor() << endl;
            cout << "Year of Publication: " << getYearOfPub() << endl;
            cout << "Publisher: " << getPublisher() << endl;
            cout << endl;
        }
    }

    void BookInformation::setAllDeques() {
        // Open JSON file
        ifstream json_file("book.json");

        // Parse JSON data
        json data;
        json_file >> data;


        // Catching Errors
        for (int i = 0; i < data.size(); i++) {
            try {
                const auto& book = data[i];
                string isbn = book["isbn"];
                string title = book["title"];
                string author = book["author"];
                string year = book["year"];
                string publisher = book["publisher"];
                isbns.push_back(isbn);
                titles.push_back(title);
                authors.push_back(author);
                years.push_back(year);
                publishers.push_back(publisher);
            } catch(exception e) {
                cout << "err" << endl;
            }
        }
    }

    int BookInformation::findIndexNum() {
        // Iterate over titles vector
        for (int i = 0; i < titles.size(); i++) {
            if (titles[i] == inputTitle) {
                return i;
            }
        }
        return -1;
    }

    void BookInformation::setAllBookValues() {
        int index = findIndexNum();
        if (index == -1) {
            ISBN = "Not found";
            bookTitle = "Not found";
            bookAuthor = "Not found";
            yearOfPub = "Not found";
            bookPublisher = "Not found";
        } else {
            ISBN = isbns[index];
            bookTitle = titles[index];
            bookAuthor = authors[index];
            yearOfPub = years[index];
            bookPublisher = publishers[index];
        }
    }

#endif /* HEADER_H_ */
