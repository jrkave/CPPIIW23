/*
*  Filename: header.h
*  Created on: 02/08/2023
*  Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include <fstream>
#include <regex>
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
// 1) Create admin menu
// 2) SQLite database operations
// 3) Login functionality

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
    typedef struct Book {
        string isbn;
        string title;
        string author;
        string year;
        string publisher;
        string genre;
        string description;
        double msrp;
        int quantity; 
    } Book;

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

    // Validation for ISBN and Year
    bool validISBNYear(string userTitle, int len1, int len2);

    // Input validation for MSRP
    bool validMSRP(string MSRP);

    // Input validation for quantity
    bool validQuantity(string quant);

    // Functor for sorting vector<Book> by value
    static bool compareByValue(const Book& a, const Book& b);

    // Display Shopping List
    void displayShoppingList(vector<Book> shoppingList);

    // Write shopping list to csv
    void writeShopList(vector<Book> shoppingList, string fileName);

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
        vector<Book> shopBook;
        vector<int> indices;

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
            cout << endl;
            while (quit != 'q') {
                cout << "Please enter a title: " << endl;
                cin.ignore();
                getline(cin, userTitle);
                if (findIndexNum(userTitle) != -1) {
                    shopBook.push_back(books[findIndexNum(userTitle)]);
                    cout << "Book successfully added. " << endl;
                }
                else {
                    cout << "Book not found. " << endl;
                }
                // Continue prompt
                cout << "Would you like to add another book to your shopping list? Enter 'q' to stop or any other letter to continue. " << endl;
                cin >> quit;
            }
            // Sort shopBook using functor
            sort(shopBook.begin(), shopBook.end(), compareByValue);
            cout << "Books added to shopping list: " << shopBook.size() << endl;
            displayShoppingList(shopBook);
            writeShopList(shopBook, shopListCSV);
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
        while (validISBNYear(addISBN, 13, 10) == false) {
            getline(cin, addISBN);
        }
        userMap["isbn"] = addISBN;
        cout << "Title to add: " << endl;
        getline(cin, addTitle);
        userMap["title"] = addTitle;
        cout << "Author to add: " << endl;
        getline(cin, addAuthor);
        userMap["author"] = addAuthor;
        cout << "Year to add: " << endl;
        getline(cin, addYear);
        while (validISBNYear(addYear, 4, 4) == false) {
            getline(cin, addYear);
        }
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
        while (validMSRP(addMSRP) == false) {
            getline(cin, addMSRP);
        }
        userMap["msrp"] = addMSRP;
        cout << "Quantity to add: " << endl;
        getline(cin, addQuantity);
        while (validQuantity(addQuantity) == false) {
            getline(cin, addQuantity);
        }
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

    // Validation for ISBN and Year
    bool BookInventory::validISBNYear(string userTitle, int len1, int len2) {
        if ((userTitle.length() != len1) && (userTitle.length() != len2)) {
            cout << "Incorrect argument length. Please try again. " << endl;
            return false;
        }
        else {
            for (int i = 0; i < userTitle.length(); i++) {
                if (isdigit(userTitle[i]) == false) {
                    cout << "Number expected, but other character found. Please try again. " << endl; 
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    // Validate MSRP
    bool BookInventory::validMSRP(string MSRP) {
        // Formats
        regex integer_expr1("[0-9][0-9]\\.[0-9][0-9]"); // 10.00
        regex integer_expr2("[0-9][0-9][0-9]\\.[0-9][0-9]"); // 100.00
        regex integer_expr3("[0-9]\\.[0-9][0-9]"); // 1.00

        // Check if valid
        if (regex_match(MSRP, integer_expr1) || regex_match(MSRP, integer_expr2) || regex_match(MSRP, integer_expr3)) {
            return true;
        }
        else {
            cout << "MSRPs must be written in the correct format, such as: 1.00, 10.00, or 100.00." << endl;
            return false;
        }
    }

    // Input validation for quantity
    bool BookInventory::validQuantity(string quant) {
        for (int i = 0; i < quant.length(); i++) {
            if (isdigit(quant[i]) == false) {
                cout << "Integer expected. Please try again. " << endl;
                return false;
            }
        }
        return true;
    }

    // Functor to sort vector<Book> by msrp value
    bool BookInventory::compareByValue(const Book& a, const Book& b) {
        return a.msrp < b.msrp;
    }

    // Write shopping list to csv
    void BookInventory::writeShopList(vector<Book> shoppingList, string fileName) {
        // Open file
        file.open(fileName, ios_base::app);
        for (int i = 0; i < shoppingList.size(); i++) {
            file << shoppingList[i].isbn + ",";
            file << shoppingList[i].title + ",";
            file << shoppingList[i].author + ",";
            file << to_string(shoppingList[i].msrp) + ",";
            file << endl;
        }
        cout << "Entry successfully written to CSV file. " << endl;
        file.close();
    }

    void BookInventory::displayShoppingList(vector<Book> shoppingList) {
        for (int i = 0; i < shoppingList.size(); i++) {
            cout << endl;
            cout << shoppingList[i].isbn << endl;
            cout << shoppingList[i].title << endl;
            cout << shoppingList[i].author << endl;
            cout << shoppingList[i].msrp << endl;
        }
    }

#endif /* BOOKINVENTORY_H_ */