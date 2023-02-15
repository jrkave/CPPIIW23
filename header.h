// header.h

#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include "json.hpp"

#ifndef HEADER_H_
#define HEADER_H_

using namespace std;
using json = nlohmann::json;

class BookInventory {
public:
    // Class constructor
    BookInventory() {
        setAllVectors();
        setAllBookValues();
    }
    // Overloaded constructor
    BookInventory(string input) {
        inputTitle = input;
        setAllVectors();
        setAllBookValues();
    }

    // Getters for ISBN
    vector<string> getISBNVec() {
        return isbns;
    }
    string getISBN() {
        return ISBN;
    }

    // Getters for Title
    string getTitle() {
        return bookTitle;
    }
    vector<string> getTitleVec() {
        return titles;
    }

    // Getters for Author
    string getAuthor() {
        return bookAuthor;
    }
    vector<string> getAuthorsVec() {
        return authors;
    }

    // Getters for Year of Publication
    string getYearOfPub() {
        return yearOfPub;
    }
    vector<string> getYearVec() {
        return years;
    }

    // Getters for Publisher
    string getPublisher() {
        return bookPublisher;
    }
    vector<string> getPublisherVec() {
        return publishers;
    }

    // Display function
    void Display();

    // Modify vectors to add apostrophes (for SQLite handling)
    vector<string> modifyVector(vector<string> updaateVector);

private:
    string inputTitle;
    vector<string> isbns;
    vector<string> titles;
    vector<string> authors;
    vector<string> years;
    vector<string> publishers;
    string ISBN;
    string bookTitle;
    string bookAuthor;
    string yearOfPub;
    string bookPublisher;
    string bookGenre;
    string bookDescription;

    // Setter for all vectors
    void setAllVectors();

    // Finds index number of title or ISBN if it exists
    int findIndexNum();

    // Setters for ISBN, Title, Author, Year of Publication, Publisher
    void setAllBookValues();
};

    void BookInventory::Display() {
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

    void BookInventory::setAllVectors() {
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
            } catch(exception *e) {
                cout << "err" << endl;
            }
        }
    }

    int BookInventory::findIndexNum() {
        // Iterate over titles vector
        for (int i = 0; i < titles.size(); i++) {
            if (titles[i] == inputTitle) {
                return i;
            }
        }
        return -1;
    }

    void BookInventory::setAllBookValues() {
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

    // Modifying vectors to prep for insertion into SQLite
    vector<string> BookInventory::modifyVector(vector<string> updateVector) {
        for (int i = 0; i < updateVector.size(); i++) {
            if (updateVector[i].find("'") != std::string::npos) {
                int pos = updateVector[i].find("'");
                updateVector[i].insert(pos+1, "'");
            }
        }
        return updateVector;
    }

    // Validate input for ISBN, Year
    bool validInput(string check, int len) {
        if (check.length() != len) {
            cout << "Invalid input. Please try again." << endl;
            return false;
        }
        for (int i = 0; i < check.length(); i++) {
            if (isdigit(check[i]) == false) {
                cout << "Invalid input. Please try again." << endl;
                return false;
            }
        }
        return true;
    }

#endif /* HEADER_H_ */
