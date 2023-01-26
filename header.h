/* 
*   Filename: header.h
*   Date: Jan. 26, 2023
*   Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include "rapidcsv.h"

#ifndef HEADER_H_
#define HEADER_H_
using namespace std;

// Declare variables
    int myIndex = 0;
    string userInput;

// Creation of vectors for ISBN, Title, Author, Publication, Publisher
    rapidcsv::Document doc("books.csv");
    vector<string> vISBN = doc.GetColumn<string>("ISBN");
    vector<string> vTitle = doc.GetColumn<string>("Book-Title");
    vector<string> vAuthor = doc.GetColumn<string>("Book-Author");
    vector<string> vYear = doc.GetColumn<string>("Year-Of-Publication");
    //vector<string> vPublisher = doc.GetColumn<string>("Publisher");

// Function to find appropriate index of vectors
    void BookInformation(string key) {
        // Finds index if key is an ISBN 
        if (isdigit(key[0])) {
            vector<string>::iterator itr = find(vISBN.begin(), vISBN.end(), key);
            if (itr != vISBN.cend()) {
                myIndex = distance(vISBN.begin(), itr);
                cout << "ISBN Number: " << vISBN[myIndex] << endl;
                cout << "Book Title: " << vTitle[myIndex] << endl;
                cout << "Book Author: " << vAuthor[myIndex] << endl;
                cout << "Year of Publication: " << vYear[myIndex] << endl; 
                //cout << "Publisher " << vPublisher[myIndex] << endl;
            }
            else {
                cout << "Book not found. " << endl;
            }
        // Finds index if key is a title
        } else {
            vector<string>::iterator itr2 = find(vTitle.begin(), vTitle.end(), key);
            if (itr2 != vTitle.cend()) {
                myIndex = distance(vTitle.begin(), itr2);
                cout << "ISBN Number: " << vISBN[myIndex] << endl;
                cout << "Book Title: " << vTitle[myIndex] << endl;
                cout << "Book Author: " << vAuthor[myIndex] << endl;
                cout << "Year of Publication: " << vYear[myIndex] << endl; 
                //cout << "Publisher: " << vPublisher[myIndex] << endl;
            }
            else {
                cout << "Book not found. " << endl;
            }
        }
    }

#endif /* HEADER_H_ */