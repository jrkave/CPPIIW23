/* 
*   Filename: main.cpp
*   Date: Jan. 26, 2023
*   Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include "header.h"
using namespace std;

int main() {

    // Welcome message
    cout << "Welcome to our book information program! Shortly, you will be prompted to input a book's title or ISBN." << endl;
    cout << "If your book is in our system, we will return the following information: " << endl;
    cout << "ISBN, Title, Author, Year of Publication, Publisher " << endl;
    cout << endl;


    // User input
    string input;
    cout << "Please enter a book's title or ISBN number: " << endl;
    cin >> input;

    // Create instance of class BookInformation using input
    BookInformation book1(input);
    book1.Display();

    // Continue prompt
    char quitProgram = ' ';
    while (quitProgram != 'q') {
        cout << "Would you like to quit? Enter q for quit, and any other key to continue. " << endl;
        cin >> quitProgram;
        if (quitProgram == 'q') {
            break;
        }
        else {
            cout << "Please enter a book's title or ISBN number: " << endl;
            cin >> input;
            BookInformation book1(input);
            book1.Display();
        }
    }
};