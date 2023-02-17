/*
*  Filename: header.h
*  Created on: 02/15/2023
*  Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include "json.hpp"

#ifndef BOOKINVENTORY_H_
#define BOOKINVENTORY_H_

using namespace std;
using json = nlohmann::json;

// TO DO //
// 1) Validate ISBN, Year for makeUserList() function (see validInput() function)
// 2) Populate field for MSRP with random values (see note in addFields() function)
// 3) Add shoppingList functionality; must be sorted by price and written to CSV/JSON file
        // Will likely be similar (and easier than) current user list functionality
// 4) Create admin menu (can be added within the executeMenu() method that is already present); see assignment details
// 5) SQLite database operations (adding, removing)
// 6) Fix current errors denoted by ** FIX ME ** comments (scroll through to see)
// 7) Login functionality
// 8) Update main.cpp with new functionalities

// Creation of class
class BookInventory {
public:
    // Class constructor
    BookInventory() {
        setVectors();
        addFields();
    }

    // Print menu function
    void printMenu();

    // Execute menu selections
    void executeMenu(int input);

    // Display function when user wishes to retrieve information about a book
    void displayBookInformation(string title);

    // Adds values to vectors
    void insertUserList(vector<string> addToUserList);

    // Get vector of values to insert
    vector<string> makeUserList();

    // Write contents of user list to csv file
    void writeToCSV(vector<string> addToUserList);

    // Print list of values to add to screen
    void displayAddValues(vector<string> addToUserList);

    // Creates a shopping list of books in database and prints the list to the user
    void makeShoppingList();

private:
    // Declaration of vectors
    vector<string> isbns;
    vector<string> titles;
    vector<string> authors;
    vector<string> years;
    vector<string> publishers;
    vector<string> genres;
    vector<string> descriptions;
    vector<string> msrps;
    vector<string> quantities;
    vector<string> addToShopList;
    vector<string> addToUserList;
    // Declarations of values in vectors
    string isbn;
    string title;
    string author;
    string year;
    string publisher;
    string genre;
    string description;
    string msrp;
    string quantity;
    // Other declarations
    int count;
    char quit;
    ofstream file;
    string csvFile = "test.csv";
    string jsonFile = "book.json";
    
    // Initializes vectors with JSON data
    int setVectors();

    // Creates vectors for added fields (genre, description, msrp, quantity)
    void addFields();

    // Finds index of vectors based on title
    int findIndexNum(string title);

    // Validate input
    bool validInput(string check, int len1, int len2);

};

    // Prints menu of selections
    void BookInventory::printMenu() {
        cout << endl;
        cout << " ====== MENU SELECTION ====== " << endl;
        cout << "To retrieve information about a book, enter 1." << endl;
        cout << "To add a list of books, enter 2." << endl;
    }

    // Execute menu selections
    /* ** FIX ME ** */
    // Add more options for menu selection once relevant methods are complete
    void BookInventory::executeMenu(int input) {
        string userTitle;
        vector<string> userVec;
        if (input == 1) {
            cout << "Please enter the title of the book you wish to retrieve. " << endl;
            cin.ignore();
            getline(cin, userTitle);
            displayBookInformation(userTitle);
        }
        else if (input == 2) {
            cout << "To add a book, we will need some additional information. Please enter the following, keeping in mind that ISBNs must be a 10 or 13-digit number, and years must be a 4-digit number. " << endl;
            cout << endl;
            while (quit != 'q') {
                /* ** FIX ME ** */
                // Currently, userVec is not being updated with new values even after new function calls to makeUserList()
                userVec = makeUserList(); 
                insertUserList(userVec);
                writeToCSV(userVec);
                displayAddValues(userVec);
                // Continue prompt
                cout << "Would you like to add another book? Enter 'q' to stop or any other letter to continue." << endl;
                cin >> quit;
            }
        }
        /* ** FIX ME ** */
        // Add option for shopping list
    }

    // Display function for retrieving values
    void BookInventory::displayBookInformation(string title) {
        int index = findIndexNum(title);
        if (findIndexNum(title) != -1) {
            cout << endl;
            cout << "ISBN: " << isbns[index] << endl;
            cout << "Title: " << titles[index] << endl;
            cout << "Author: " << authors[index] << endl;
            cout << "Year of publication: " << years[index] << endl;
            cout << "Publisher: " << publishers[index] << endl;
            cout << "Genre: " << genres[index] << endl;
            cout << "Description: " << descriptions[index] << endl;
            cout << "MSRP: " << msrps[index] << endl;
            cout << "Quantity: " << quantities[index] << endl;
            cout << endl;
        }
        else {
            cout << "Book not found. " << endl;
            cout << endl;
        }
    }

    // Sets vectors with parsed json data
    int BookInventory::setVectors() {

        // Opens JSON file
        ifstream json_file(jsonFile);
        // Validation for opening of file
        if (json_file.is_open()) {
            cout << "Starting system... " << endl;
        }
        else {
            cout << "Error opening the file. " << endl;
            return -1;
        }

        // Creates JSON object "data" and parses file
        json data;
        json_file >> data;

        // Updates vectors with parsed data, includes error handling
        for (int i = 1; i < data.size(); i++) {
            try {
                const auto& book = data[i];
                // Adds data to vector
                isbns.push_back(book["isbn"]);
                titles.push_back(book["title"]);
                authors.push_back(book["author"]);
                years.push_back(book["year"]);
                publishers.push_back(book["publisher"]);
            } catch (exception *e) {
                cout << "Error in parsing file. " << endl;
            }
        }
        return 0;
    }

    // Adds fields for genre, description, MSRP, quantity
    void BookInventory::addFields() {
        for (int i = 0; i < titles.size(); i++) {
            genres.push_back("NULL");
            descriptions.push_back("NULL");
            quantities.push_back("NULL");
            /* ** FIX ME ** */
            // Fill in MSRP data with random number (type-cast as string before insertion)
            msrps.push_back("NULL");
        }
    }

    // Returns the index of a given title if it exists
    int BookInventory::findIndexNum(string title) {
        for (int i = 0; i < titles.size(); i++) {
            if (titles[i] == title) {
                return i;
            }
        }
        return -1;
    }

    // Gathers input for user list and returns it as vector
    vector<string> BookInventory::makeUserList() {
        string addISBN, addTitle, addAuthor, addYear, addPublisher, addGenre, addDescription, addMSRP, addQuantity;
        // Get values
        cout << "ISBN to add: " << endl;
        cin.ignore();
        getline(cin, addISBN);
        addToUserList.push_back(addISBN);
        cout << "Title to add: " << endl;
        getline(cin, addTitle);
        addToUserList.push_back(addTitle);
        cout << "Author to add: " << endl;
        getline(cin, addAuthor);
        addToUserList.push_back(addAuthor);
        cout << "Year to add: " << endl;
        getline(cin, addYear);
        addToUserList.push_back(addYear);
        cout << "Publisher to add: " << endl;
        getline(cin, addPublisher);
        addToUserList.push_back(addPublisher);
        cout << "Genre to add: " << endl;
        getline(cin, addGenre);
        addToUserList.push_back(addGenre);
        cout << "Description to add: " << endl;
        getline(cin, addDescription);
        addToUserList.push_back(addDescription);
        cout << "MSRP to add: " << endl;
        getline(cin, addMSRP);
        addToUserList.push_back(addMSRP);
        cout << "Quantity to add: " << endl;
        getline(cin, addQuantity);
        addToUserList.push_back(addQuantity);
    
        return addToUserList;
    }

    // Inserts values from makeUserList to vectors
    void BookInventory::insertUserList(vector<string> addToUserList) {
        for (int i = 0; i < addToUserList.size(); i++) {
            isbns.push_back(addToUserList[0]);
            titles.push_back(addToUserList[1]);
            authors.push_back(addToUserList[2]);
            years.push_back(addToUserList[3]);
            publishers.push_back(addToUserList[4]);
            genres.push_back(addToUserList[5]);
            descriptions.push_back(addToUserList[6]);
            msrps.push_back(addToUserList[7]);
            quantities.push_back(addToUserList[8]);
        }
        cout << "Data inserted successfully. " << endl;
    }

    // Write csv file for User List
    void BookInventory::writeToCSV(vector<string> addToUserList) {
        // Open file
        file.open(csvFile);
        for (int i = 0; i < addToUserList.size(); i++) {
            file << addToUserList[i] << ",";
        }
        file << endl;
        cout << "Entry successfully written to CSV file. " << endl;
        file.close();
    }

    // Display values of list user wishes to add
    void BookInventory::displayAddValues(vector<string> addToUserList) {
        cout << "ISBN: " << addToUserList[0] << endl;
        cout << "Title: " << addToUserList[1] <<endl;
        cout << "Author: " << addToUserList[2] << endl;
    }

    // Validate input for ISBN, Year
    bool BookInventory::validInput(string check, int len1, int len2) {
        if (check.length() != len1 && check.length() != len2) {
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

#endif /* BOOKINVENTORY_H_ */