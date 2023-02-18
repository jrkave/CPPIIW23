/*
*  Filename: bookinventory.h
*  Created on: 02/15/2023
*  Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
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
    string userListCSV = "userList.csv";
    string shopListCSV = "shoppingList.csv";
    string jsonFile = "book.json";
    
    // Initializes vectors with JSON data
    int setVectors();

    // Creates vectors for added fields (genre, description, msrp, quantity)
    void addFields();

    // Finds index of vectors based on title
    int findIndexNum(string title);

    // Validate input
    bool validInput(string check, int len1, int len2);

    // Display function when user wishes to retrieve information about a book
    void displayBookInformation(string title);

    // Get vector of values to insert to user list
    unordered_map<string, string> makeUserList();

    // Adds values of user list to vectors
    void insertUserList(unordered_map<string, string> userMap);

    // Write contents of user list to csv file
    void writeUserList(unordered_map<string, string> userMap, string fileName);

    // Print user list to screen
    void displayUserList(vector<string> userList);

};

    // Prints menu of selections
    void BookInventory::printMenu() {
        cout << endl;
        cout << " ====== MENU SELECTION ====== " << endl;
        cout << "To retrieve information about a book, enter 1." << endl;
        cout << "To add a list of books, enter 2." << endl;
        cout << "To create a shopping list, enter 3." << endl;
    }

    // Execute menu selections
    void BookInventory::executeMenu(int input) {
        // Declarations
        string userTitle;
        unordered_map<string, string> userMap;
        map<int, string> shoppingList;
        vector<string> userList;

        // Retrieval of book information
        if (input == 1) {
            cout << endl;
            cout << "Please enter the title of the book you wish to retrieve. " << endl;
            cin.ignore();
            getline(cin, userTitle);
            displayBookInformation(userTitle);
        }
        // Adding books to user list
        else if (input == 2) {
            cout << endl;
            cout << "To add a book, we will need some additional information. Please enter the following, keeping in mind that ISBNs must be a 10 or 13-digit number, and years must be a 4-digit number. " << endl;
            cout << endl;
            while (quit != 'q') {
                userMap = makeUserList(); 
                userList.push_back(userMap["isbn"]);
                userList.push_back(userMap["title"]);
                userList.push_back(userMap["author"]);
                insertUserList(userMap);
                writeUserList(userMap, userListCSV);
                // Continue prompt
                cout << "Would you like to add another book? Enter 'q' to stop or any other letter to continue." << endl;
                cin >> quit;
            }
            cout << endl;
            // Displays ISBN, title, author, # of books added
            cout << "List of books added: " << endl;
            displayUserList(userList);
        }
        // Creating shopping list
        else if (input == 3) {
            // FIX ME
            cout << endl;
            cout << "To make a shopping list, please enter the titles of the books you wish to add. " << endl;
            cin.ignore();
            getline(cin, userTitle);
            cout << endl;
            while (quit != 'q') {
                if (findIndexNum(userTitle) == -1) {
                    cout << "Book not found. " << endl;
                }
                else {
                    // Stores values as key, value pair {indexNum, MSRP}; sorting by MSRP value will come later
                    shoppingList.insert({findIndexNum(userTitle), msrps[findIndexNum(userTitle)]});
                    cout << "FIX ME" << endl;
                }
            }
        }
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
    unordered_map<string, string> BookInventory::makeUserList() {
        unordered_map<string, string> userMap;
        string addISBN, addTitle, addAuthor, addYear, addPublisher, addGenre, addDescription, addMSRP, addQuantity;
        // Get values
        cout << "ISBN to add: " << endl;
        cin.ignore();
        getline(cin, addISBN);
        userMap["isbn"] = addISBN;
        cout << "Title to add: " << endl;
        getline(cin, addTitle);
        userMap["title"] = addTitle;
        cout << "Author to add: " << endl;
        getline(cin, addAuthor);
        userMap["author"] = addAuthor;
        cout << "Year to add: " << endl;
        getline(cin, addYear);
        userMap["year"] = addYear;
        cout << "Publisher to add: " << endl;
        getline(cin, addPublisher);
        userMap["publisher"] = addPublisher;
        cout << "Genre to add: " << endl;
        getline(cin, addGenre);
        userMap["genre"] = addGenre;
        cout << "Description to add: " << endl;
        getline(cin, addDescription);
        userMap["description"] = addDescription;
        cout << "MSRP to add: " << endl;
        getline(cin, addMSRP);
        userMap["msrp"] = addMSRP;
        cout << "Quantity to add: " << endl;
        getline(cin, addQuantity);
        userMap["quantity"] = addQuantity;
        return userMap;
    }

    // Inserts values from makeUserList to vectors
    void BookInventory::insertUserList(unordered_map<string, string> userMap) {
        isbns.push_back(userMap["isbn"]);
        titles.push_back(userMap["title"]);
        authors.push_back(userMap["author"]);
        years.push_back(userMap["year"]);
        publishers.push_back(userMap["publisher"]);
        genres.push_back(userMap["genre"]);
        descriptions.push_back(userMap["description"]);
        msrps.push_back(userMap["msrp"]);
        quantities.push_back(userMap["quantity"]);
        cout << "Data inserted successfully. " << endl;
    }

    // Write csv file for User List
    void BookInventory::writeUserList(unordered_map<string, string> userMap, string fileName) {
        // Open file
        file.open(fileName, ios_base::app);
        file << userMap["isbn"] + ",";
        file << userMap["title"] + ",";
        file << userMap["author"] + ",";
        file << userMap["year"] + ",";
        file << userMap["publisher"] + ",";
        file << userMap["genre"] + ",";
        file << userMap["description"] + ",";
        file << userMap["msrp"] + ",";
        file << userMap["quantity"] + ",";
        file << endl;
        cout << "Entry successfully written to CSV file. " << endl;
        file.close();
    }

    // Display values of list user wishes to add
    void BookInventory::displayUserList(vector<string> userList) {
        for (int i = 0; i < userList.size(); i += 3) {
            cout << "ISBN: " << userList[i] << endl;
            cout << "Title: " << userList[i+1] << endl;
            cout << "Author: " << userList[i+2] << endl;
            cout << endl;
        }
        cout << "Total items: " << userList.size() / 3 << endl;
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