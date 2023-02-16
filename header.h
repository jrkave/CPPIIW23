// header.h

#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <list>
#include <algorithm>
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
        addFields();
        setAllBookValues();
    }

    // Overloaded constructor
    BookInventory(string input) {
        inputTitle = input;
        setAllVectors();
        addFields();
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

    // Getters for Genre
    string getGenre() {
        return bookGenre;
    }
    vector<string> getGenreVec() {
        return genres;
    }

    // Getter for Description
    string getDescription() {
        return bookDescription;
    }
    vector<string> getDescriptionVec() {
        return descriptions;
    }

    void addToVec(string addISBN, string addTitle, string addAuthor, string addYear, string addPublisher, string addGenre, string addDescription);

    void removeFromVec(string delTitle);

    // Display function
    void Display();

    // Modify vectors to add apostrophes (for SQLite handling)
    vector<string> modifyVector(vector<string> updateVector);

    // Make a list for user of books they want to add
    int makeList();

    // Display user list
    void DisplayList();

private:
    char toContinue;
    int count;
    string inputTitle;
    vector<string> isbns;
    vector<string> titles;
    vector<string> authors;
    vector<string> years;
    vector<string> publishers;
    vector<string> genres;
    vector<string> descriptions;
    vector<vector<string>> big;
    vector<string> small;
    string ISBN;
    string bookTitle;
    string bookAuthor;
    string yearOfPub;
    string bookPublisher;
    string bookGenre;
    string bookDescription;

    // Setter for all vectors from json parsing
    void setAllVectors();

    // Add fields for genre, description
    void addFields();

    // Finds index number of title if it exists
    int findIndexNum();

    // Overloaded findIndexNum
    int findIndexNum(string title);

    // Setters for ISBN, Title, Author, Year of Publication, Publisher, Genre, Description
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
            cout << "Genre: " << getGenre() << endl;
            cout << "Description: " << getDescription() << endl;
            cout << endl;
        }
    }

    void BookInventory::addFields() {
        for (int i = 0; i < 271361; i++) {
            genres.push_back("Null");
            descriptions.push_back("Null");
        }
    }

    void BookInventory::setAllVectors() {
        // Open JSON file
        ifstream json_file("book.json");

        // Parse JSON data
        json data;
        json_file >> data;

        // Catching Errors
        for (int i = 1; i < data.size(); i++) {
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

    int BookInventory::findIndexNum(string title) {
        for (int i = 0; i < titles.size(); i++) {
            if (titles[i] == title) {
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
            bookGenre = "Not found";
            bookDescription = "Not found";
        } else {
            ISBN = isbns[index];
            bookTitle = titles[index];
            bookAuthor = authors[index];
            yearOfPub = years[index];
            bookPublisher = publishers[index];
            bookGenre = genres[index];
            bookDescription = descriptions[index];
        }
    }

    // Add data to vector
    void BookInventory::addToVec(string addISBN, string addTitle, string addAuthor, string addYear, string addPublisher, string addGenre, string addDescription) {
        isbns.push_back(addISBN);
        titles.push_back(addTitle);
        authors.push_back(addAuthor);
        years.push_back(addYear);
        publishers.push_back(addPublisher);
        genres.push_back(addGenre);
        descriptions.push_back(addDescription);
        cout << "Data successfully added to vectors. " << endl;
    }

    // FIX ME: Segmentation fault if removing from last
    // Remove data from vector
    void BookInventory::removeFromVec(string delTitle) {
        if (findIndexNum(delTitle) == (titles.size() - 1)) {
            // If input title is last element from vector, delete it
            isbns.pop_back();
            titles.pop_back();
            authors.pop_back();
            years.pop_back();
            publishers.pop_back();
            genres.pop_back();
            descriptions.pop_back();
            cout << "Data removed succesfullly from vectors. " << endl;
        }
        else if (findIndexNum(delTitle) == -1 ) {
            // If findIndex num returns -1, the book was not found in titles vector
            cout << "Book not found. " << endl;
        }
        else {
            // Erase elements by index
            titles.erase(titles.begin() + findIndexNum(delTitle));
            isbns.erase(isbns.begin() + findIndexNum(delTitle));
            authors.erase(authors.begin() + findIndexNum(delTitle));
            years.erase(years.begin() + findIndexNum(delTitle));
            publishers.erase(titles.begin() + findIndexNum(delTitle));
            genres.erase(isbns.begin() + findIndexNum(delTitle));
            descriptions.erase(titles.begin() + findIndexNum(delTitle));
        }
    }

    // Make a list of lists to store data user wants to append
    int BookInventory::makeList() {
        while (toContinue != 'q') {
            cout << "Please enter a title. " << endl;
            cin.ignore();
            getline(cin, bookTitle);
            if (findIndexNum(bookTitle) == -1) {
                cout << "Book not found. " << endl;
            }
            // Add isbn, author, title to list
            else {
                small.push_back(isbns[findIndexNum(bookTitle)]);
                small.push_back(titles[findIndexNum(bookTitle)]);
                small.push_back(authors[findIndexNum(bookTitle)]);
                cout << "Successfullly added to list. " << endl;
            }
            cout << "Would you like to add another book? Enter 'q' to quit, or any other letter to continue. " << endl;
            cin >> toContinue;
        }
        big.push_back(small);
        return (small.size() / 3);
    }

    // Display List
    void BookInventory::DisplayList() {
        // Iterate through the outer list and print the values of the inner lists
        for (auto outer_it = big.begin(); outer_it != big.end(); ++outer_it) {
            // Iterate through the inner list and print its values, skipping the first element
            for (auto inner_it = outer_it->begin(); inner_it != outer_it->end(); ++inner_it) {
                cout << *inner_it << " ";
            }
            cout << endl;
        cout << endl;
        }
        cout << "Total items: " << makeList() << endl;
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

#endif /* HEADER_H_ */
