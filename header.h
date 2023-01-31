/* 
*   Filename: header.h
*   Date: Jan. 26, 2023
*   Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "rapidcsv.h"

#ifndef HEADER_H_
#define HEADER_H_
using namespace std;

// Instantiation of Doc class from rapidcsv
rapidcsv::Document doc("books.csv");

class BookInformation {
public:
    // Class constructor
    BookInformation(string input) {
        titleOrISBN = input;
        setAllVectors();
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

    void Display();

private:
    // Class variables
    int myIndex = 0;
    string titleOrISBN;
    vector<string> vISBN;
    vector<string> vTitle;
    vector<string> vAuthor;
    vector<string> vYearOfPub;
    vector<string> vPublisher;
    vector<int> vErrors;
    string ISBN;
    string bookTitle;
    string bookAuthor;
    string yearOfPub;
    string bookPublisher;

    // Setter for vectors (ISBN, Book-Title, Book-Author, Year-Of-Publication) using rapidcsv
    void setAllVectors();

    // Finds index number of title or ISBN if it exists
    int findIndexNum();

    // Setter for ISBN, Title, Author, Year of Publication
    void setAllBookValues();

};

    void BookInformation::Display() {
        if (getISBN() == "Not found ") {
            cout << "Invalid entry or book not found. Please try again. " << endl;
        }
        else {
            cout << "ISBN: " << getISBN() << endl;
            cout << "Title: " << getTitle() << endl;
            cout << "Author: " << getAuthor() << endl;
            cout << "Year of Publication: " << getYearOfPub() << endl;
            cout << endl;
        }
    }

    void BookInformation::setAllVectors() {
        vISBN = doc.GetColumn<string>("ISBN");
        vTitle = doc.GetColumn<string>("Book-Title");
        vAuthor = doc.GetColumn<string>("Book-Author");
        vYearOfPub = doc.GetColumn<string>("Year-Of-Publication");
        /*
        try {
            vPublisher = doc.GetColumn<string>("Publisher");
        }
        catch(out_of_range& orr) {
            vErrors.push_back(orr);
        }
        */
    }

    int BookInformation::findIndexNum() {
        // Finds index if key is an ISBN
        if (isdigit(titleOrISBN[0])) {
            vector<string>::iterator itr = find(vISBN.begin(), vISBN.end(), titleOrISBN);
            if (itr != vISBN.cend()) {
                return distance(vISBN.begin(), itr);
            }
            else {
                return 0;
            }
            // Finds index if key is a title
        } else {
            vector<string>::iterator itr2 = find(vTitle.begin(), vTitle.end(), titleOrISBN);
            if (itr2 != vTitle.cend()) {
                return distance(vTitle.begin(), itr2);
            }
            else {
                return 0;
            }
        }
    }

    void BookInformation::setAllBookValues() {
        int index = findIndexNum();
        if (index == 0) {
            ISBN = "Not found ";
            bookTitle = "Not found ";
            bookAuthor = "Not found ";
            yearOfPub = "Not found ";
        }
        else {
            ISBN = vISBN[index];
            bookTitle = vTitle[index];
            bookAuthor = vAuthor[index];
            yearOfPub = vYearOfPub[index];
        }
    };

#endif /* HEADER_H_ */
