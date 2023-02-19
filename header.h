/*
*  Filename: bookinventory.h
*  Created on: 02/15/2023
*  Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
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
        setVector();
    }

    // Print menu function
    void printMenu();

    // Execute menu selections
    void executeMenu(int input);

private:
    // Declaration of struct and vector of structs
    struct Book {
        string isbn;
        string title;
        string author;
        string year;
        string publisher;
        string genre;
        string description;
        double msrp;
        int quantity; 
    };
    Book book;
    vector<Book> books;
    // Other declarations
    int count = 0;
    char quit = 'm';
    ofstream file;
    string userListCSV = "userList.csv";
    string shopListCSV = "shoppingList.csv";
    string jsonFile = "book.json";
    
    // Initializes vectors with JSON data
    int setVector();

    // Retrieves and displays information about a book
    void displayBookInformation(string title);

    // Given a title, returns value of index of vector<Book> if book exists
    int findIndexNum(string title);

    // Get vector of values to insert to user list
    unordered_map<string, string> makeUserList();

    // Adds values of user list to vectors
    void insertUserList(unordered_map<string, string> userMap);

    // Writes user list to CSV file
    void writeUserList(unordered_map<string, string> userMap, string fileName);

    // Displays User List
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
        vector<string> userList;

        // Retrieval of book information
        if (input == 1) {
            cout << endl;
            while (quit != 'q') {
                cout << "Please enter the title of the book you wish to retrieve. " << endl;
                cin.ignore();
                getline(cin, userTitle);
                displayBookInformation(userTitle);
                cout << endl;
                // Continue prompt
                cout << "Would you like to retrieve another book? Enter 'q' to stop or another other letter to continue." << endl;
                cin >> quit;
            }
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
        }
    }

    // Sets vectors with parsed json data
    int BookInventory::setVector() {

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

        srand(time(NULL));
        // Updates vectors with parsed data, includes error handling
        for (int i = 1; i < data.size(); i++) {
            try {
                const auto& bookList = data[i];
                // Add to struct
                book.isbn = bookList["isbn"];
                book.title = bookList["title"];
                book.author = bookList["author"];
                // Convert string to int
                book.year = bookList["year"];
                book.publisher = bookList["publisher"];
                book.genre = "NULL";
                book.description = "NULL";
                int randQuant = rand() % 101;
                book.quantity = randQuant;
                double randMSRP = (103.97 - 34.99) * ((double)rand() / (double)RAND_MAX) + 34.99;
                book.msrp = randMSRP;
                // Add struct to vector
                books.push_back(book);
            } catch (exception *e) {
                cout << "Error in parsing file. " << endl;
            }
        }
        return 0;
    }

    // Display function for retrieving values
    void BookInventory::displayBookInformation(string title) {
        int index = findIndexNum(title);
        if (index == -1) {
            cout << "Book not found. " << endl;
        }
        else {
            cout << endl;
            cout << "ISBN: " << books[index].isbn << endl;
            cout << "Title: " << books[index].title << endl;
            cout << "Author: " << books[index].author << endl;
            cout << "Year of publication: " << books[index].year << endl;
            cout << "Publisher: " << books[index].publisher << endl;
            cout << "Genre: " << books[index].genre << endl;
            cout << "Description: " << books[index].description << endl;
            cout << "MSRP: $" << books[index].msrp << endl;
            cout << "Quantity: " << books[index].quantity << endl;
        }
    }

    // Checks if book exists within vector<Book> books;
    int BookInventory::findIndexNum(string title) {
        for (int i = 0; i < books.size(); i++) {
            if (books[i].title == title) {
                return i;
            }
        }
        return -1;
    }

    // Gathers input for user list and returns it as vector
    unordered_map<string, string> BookInventory::makeUserList() {
        // Declare variables
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

    // Inserts values from makeUserList to vector<Book> book
    void BookInventory::insertUserList(unordered_map<string, string> userMap) {
        Book newBook;
        newBook.isbn = userMap["isbn"];
        newBook.title = userMap["title"];
        newBook.author = userMap["author"];
        newBook.year = userMap["year"];
        newBook.publisher = userMap["publisher"];
        newBook.genre = userMap["genre"];
        newBook.description = userMap["description"];
        newBook.msrp = stod(userMap["msrp"]);
        newBook.quantity = stoi(userMap["quantity"]);
        books.push_back(newBook);
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

#endif /* BOOKINVENTORY_H_ */