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
#include <unordered_map>
#include <algorithm>
#include "json.hpp"
#include "sqlite3.h"
#include "login.h"

#ifndef HEADER_H_
#define HEADER_H_

using namespace std;
using json = nlohmann::json;

// Setup of struct
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

// Hold username and password
    struct User {
    	string username;
    	string password;
    };

// Sales receipt
    typedef struct Receipt {
        string purchasedBook = "Unavailable";
        int amount = 0;
        double msrp = 0.0;
        double tax = 0.0;
        double total = 0.0;
    } Receipt;

// Creation of class
class BookInventory {
public:
    // Declaration of variables
    Receipt receipt;
    vector<Receipt> receipts;
    Book book;
    vector<Book> books;

    // Class constructor
    BookInventory() {
        setVector();
    }

    // Print and execute menu functions
    void printMenu();
    void printAdminMenu();
    void executeMenu(int input);
    void executeAdminMenu(int adminInput);

private:
    // Other declarations
    int count = 0;
    char quit = 'm';
    ofstream file;
    string userListCSV = "userList.csv";
    string shopListCSV = "shoppingList.csv";
    string removeListCSV = "removeList.csv";
    string jsonFile = "book.json";
    string CSVfilename = "login.csv";
    
    // Initializes vector<Book> with JSON data
    int setVector();

    // Retrieves and displays information about a book
    void displayBookInformation(string title);

    // Given a title, returns value of index of vector<Book> if book exists
    int findIndexNum(string title);

    // User List Functions
    unordered_map<string, string> makeUserList(); // stores values to insert into a map
    void insertUserList(unordered_map<string, string> userMap, sqlite3 *db); // inserts values from map into vector<Book>
    void writeUserList(unordered_map<string, string> userMap, string fileName); // writes user list to CSV file
    void displayUserList(vector<string> userList); // displays user list

    // Shopping  and Remove List Functions
    static bool compareByValue(const Book& a, const Book& b); // functor to sort vector<Book> by MSRP value
    void writeList(vector<Book> list, string fileName); // writes shopping / removed lists to CSV file
    void displayList(vector<Book> list); // displays list of books in vector<Book> (also used for removing books)

    // Input validation Functions
    bool validISBNYear(string userTitle, int len1, int len2); // validates ISBN and year
    bool validMSRP(string MSRP); // validates MSRP using pattern matches in regex lib
    bool validQuantity(string quant); // validates quantity

    // Database Functions
    int removeData(sqlite3 *db, const char *title);
    int addData(sqlite3 *db, const char *isbn, const char *title, const char *author, const char *year, const char *publisher, const char *genre, const char *description, double msrp, int quantity);

    // Admin menu functions
    void addNewUser(string username, string password);
    void changeUserPassword(string username, string password);

    // Purchasing / shopper functions
    int addShopper(sqlite3 *db, const char *name, const char *email);
    bool doesShopperExist(sqlite3 *db, const char *name);
    Receipt purchaseBooks(string title);
    void displayReceipt(Receipt receipt);
    int addShopperTotal(sqlite3 *db, const char *name, double total);

    // Adding deals
    bool hasValidCoupon(string coupon);

};
    // Prints menu of selections
    void BookInventory::printMenu() {
        std::cout << std::endl;
        std::cout << " ====== MENU SELECTION ====== " << std::endl;
        std::cout << "To retrieve information about a book, enter 1." << std::endl;
        std::cout << "To add books to inventory, enter 2." << std::endl;
        std::cout << "To remove books from inventory, enter 3. " << std::endl;
        std::cout << "To create a shopping list, enter 4." << std::endl;
        std::cout << "To see the admin menu, enter 5." << std::endl;
        std::cout << "To purchase a book, enter 6." << std::endl;
    }

    void BookInventory::printAdminMenu() {
        std::cout << std::endl;
        std::cout << " ====== ADMIN MENU SELECTION ====== " << std::endl;
        std::cout << "To add a user, enter 1. " << std::endl;
        std::cout << "To change a password, enter 2. " << std::endl;
    }

    // Execute admin menu selections 
    void BookInventory::executeAdminMenu(int adminInput) {
    	// Declarations
    	string username, password;
    	// Option 1: Add new user to login.csv
        if (adminInput == 1) {
            std::cout << "Please enter username for new user:" << std::endl;
            std::cin >> username;
            std::cout << "Please enter password for new user:" << std::endl;
            std::cin >> password;
            addNewUser(username, password);

        }
        // Option 2: Change password of existing account
        else if (adminInput == 2) {
        	std::cout << "Enter username associated with password to be changed: " << std::endl;
        	std::cin >> username;
        	std::cout << "Enter new password: " << std::endl;
        	std::cin >> password;
        	changeUserPassword(username, password);
        }
        else {
            std::cout << "Invalid input. " << std::endl;
        }
    }

    // Execute menu selections
    void BookInventory::executeMenu(int input) {
        // Declarations
        string userTitle;
        unordered_map<string, string> userMap;
        vector<string> userList;
        vector<Book> shopBook;
        vector<Book> removeBook;

        // Open databasse
        sqlite3 *db;
        int rc = sqlite3_open("books.db", &db);
        if (rc != SQLITE_OK) {
            cout << "Error opening database: " << sqlite3_errmsg(db) << endl;
        }

        // OPTION 1: Retrieval of book information
        if (input == 1) {
            std::cout << std::endl;
            while (quit != 'q') {
                std::cout << "Please enter a title: " << std::endl;
                cin.ignore();
                std::getline(cin, userTitle);
                displayBookInformation(userTitle);
                std::cout << std::endl;
                // Continue prompt
                std::cout << "Would you like to retrieve another book? Enter 'q' to stop or another other letter to continue." << std::endl;
                cin >> quit;
            }
        }
        // OPTION 2: Adding books to inventory
        else if (input == 2) {
            std::cout << std::endl;
            std::cout << "To add a book, we will need some additional information. Please enter the following, keeping in mind that ISBNs must be a 10 or 13-digit number and years must be a 4-digit number. " << std::endl;
            std::cout << "Additionally, if your entry contains a special character, please contain the entry in double-quotes. " << std::endl;
            std::cout << "For example, a title such as Jane's Book should be written as \"Jane's Book\"." << std::endl;
            std::cout << std::endl;
            while (quit != 'q') {
                userMap = makeUserList(); 
                userList.push_back(userMap["isbn"]);
                userList.push_back(userMap["title"]);
                userList.push_back(userMap["author"]);

                insertUserList(userMap, db); // adds book to vector<Book> and database
                writeUserList(userMap, userListCSV); // writes to CSV
                // Continue prompt
                std::cout << "Would you like to add another book? Enter 'q' to stop or any other letter to continue." << std::endl;
                cin >> quit;
            }
            std::cout << std::endl;
            // Displays ISBN, title, author, # of books added
            std::cout << "List of books added: " << std::endl;
            displayUserList(userList);
        }
        // OPTION 3: Remove books from inventory
        else if (input == 3) {
            std::cout << std::endl;
            while (quit != 'q') {
                std::cout << "Please enter a title: " << std::endl;
                cin.ignore();
                std::getline(cin, userTitle);
                int index = findIndexNum(userTitle);
                if (index != -1) {
                    // If element is at back of books, pop back
                    if (index == books.size() - 1) {
                        removeBook.push_back(books[index]); // add to vector of removed books
                        books.pop_back(); // delete from vector<Book> books
                        std::cout << "Book successfully removed. " << std::endl;
                        removeData(db, userTitle.c_str()); // delete from database
                    }
                    else {
                        removeBook.push_back(books[index]); // add to vector of removed books
                        books.erase(books.begin() + index); // delete from vector<Book> books
                        std::cout << "Book successfully removed. " << std::endl;
                        removeData(db, userTitle.c_str()); // delete from database
                    }
                }
                else {
                    std::cout << "Book not found. " << std::endl;
                }
                // Continue prompt
                std::cout << "Would you like to remove another book? Enter 'q' to stop or any other letter to continue." << endl;
                cin >> quit;
            }
            std::cout << std::endl;
            // Displays list of books removed
            std::cout << "Books removed from inventory: " << removeBook.size() << std::endl;
            writeList(removeBook, removeListCSV);
            displayList(removeBook);
        }
        // OPTION 4: Creating shopping list
        else if (input == 4) {
            std::cout << std::endl;
            while (quit != 'q') {
                std::cout << "Please enter a title: " << std::endl;
                cin.ignore();
                std::getline(cin, userTitle);
                if (findIndexNum(userTitle) != -1) {
                    shopBook.push_back(books[findIndexNum(userTitle)]);
                    std::cout << "Book successfully added. " << std::endl;
                }
                else {
                    std::cout << "Book not found. " << std::endl;
                }
                // Continue prompt
                std::cout << "Would you like to add another book to your shopping list? Enter 'q' to stop or any other letter to continue. " << std::endl;
                cin >> quit;
            }
            // Sort shopBook using functor
            sort(shopBook.begin(), shopBook.end(), compareByValue);
            std::cout << "Books added to shopping list: " << shopBook.size() << std::endl;
            displayList(shopBook);
            writeList(shopBook, shopListCSV);
        }
        // OPTION 5: Admin Menu
        else if (input == 5) {
            printAdminMenu();
            int adminInput;
            cin >> adminInput;
            executeAdminMenu(adminInput);
        }
        // OPTION 6: Purchase Book
        else if (input == 6) {
            Receipt receipt;
            vector<Receipt> receipts;
            string title;
            string name;
            string email;
            string modEmail;
            double total = 0.0;
            char couponChoice = 'k';
            string coupon;
            bool validCoupon;

            // Get name, emails
            std::cout << "Please enter your full name: " << std::endl;
            cin.ignore();
            getline(cin, name);
            std::cout << "Please enter your email: " << std::endl;
            cin >> email;
            addShopper(db, name.c_str(), email.c_str()); // add shopper to database

            // Purchase books
            while (quit != 'q') {
                std::cout << "Please enter the title of the book you'd like to purchase. " << std::endl;
                cin.ignore();
                getline(cin, title);
                receipt = purchaseBooks(title);
                receipts.push_back(receipt);
                // Continue prompt
                std::cout << "Would you like to purchase another book? Enter 'q' to stop or any other letter to continue. " << std::endl;
                cin >> quit;
            }
            // Coupon code 
            std::cout << "Do you have a coupon code? Enter y/n." << std::endl;
            std::cin >> couponChoice;
            while (couponChoice != 'y' && couponChoice != 'n') {
                std::cout << "Invalid entry. Please enter y for yes, n for no." << std::endl;
                std::cin >> couponChoice;
            }
            // Apply coupon or not
            if (couponChoice == 'y') {
                std::cout << "Please enter your code: " << std::endl;
                std::cin >> coupon;
                validCoupon = hasValidCoupon(coupon);
                if (validCoupon == false) {
                    std::cout << "Sorry, that is not a valid coupon code." << std::endl;
                }
                else {
                    std::cout << "Your coupon has been applied." << std::endl;
                }
            }

            // Sales receipt
            std::cout << std::endl;
            std::cout << " === SALES RECEIPT ===" << std::endl;
            std::cout << std::endl;
            std::cout << "Customer: " << name << std::endl;
            std::cout << "Email: " << email << std::endl;
            std::cout << std::endl;
            for (int i = 0; i < receipts.size(); i++) {
                if (receipts[i].purchasedBook == "Unavailable") {
                    continue;
                }
                else {
                    displayReceipt(receipts[i]);
                    std::cout << std::endl;
                    total += receipts[i].total;
                }
            }

            // Add coupon discount if they had one
            if (validCoupon == true) {
                total = round((total * 0.8) * 100) / 100.0;
                std::cout << "TOTAL WITH COUPON: $" << total << std::endl;
                addShopperTotal(db, name.c_str(), total);
            }
            else {
                total = round(total * 100) / 100.0;
                std::cout << "TOTAL: $" << total << std::endl; 
                addShopperTotal(db, name.c_str(), total);
            }
        }
    }
    
    // Parse login CSV line into User struct
    User parseUser(string line) {
    	User user;
    	size_t pos = 0;
    	string token;
    	int i = 0;
    	while ((pos = line.find(",")) != string::npos) {
    		token = line.substr(0, pos);
    		if (i == 0) {
    			user.username = token;
    		}
    		else {
    			user.password = token;
    		}
    		line.erase(0, pos + 1);
    		i++;
    	}
    	if (i == 1) {
    		user.password = line;
    	}
    	return user;
    }

    // Read login CSV into vector
    vector<User> readCSV(string filename) {
    	vector<User> users;
    	ifstream file(filename);
    	if (file) {
    		string line;
    		while (getline(file, line)) {
    			User user = parseUser(line);
    			users.push_back(user);
    		}
    		file.close();
    	}
    	return users;
    }
    void writeCSV(string filename, vector<User> users) {
    	ofstream file(filename);
    	if (file) {
    		for (int i=0; i < users.size(); i++) {
    			file << users[i].username << "," << users[i].password << endl;
    		}
    		file.close();
    	}
    }
    
    // Sets vectors with parsed json data
    int BookInventory::setVector() {

        // Opens JSON file
        ifstream json_file(jsonFile);
        // Validation for opening of file
        if (json_file.is_open()) {
            std::cout << "Starting system... " << std::endl;
        }
        else {
            std::cout << "Error opening the file. " << std::endl;
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
                book.year = bookList["year"];
                book.publisher = bookList["publisher"];
                book.genre = "NULL";
                book.description = "NULL";
                int randQuant = rand() % (56 - 2 + 1) + 2;
                book.quantity = randQuant;
                double randMSRP = (103.97 - 34.99) * ((double)rand() / (double)RAND_MAX) + 34.99;
                book.msrp = randMSRP;
                // Add struct to vector<Book>
                books.push_back(book);
            } catch (exception *e) {
                std::cout << "Error in parsing file. " << std::endl;
            }
        }
        return 0;
    }

    // Display function for retrieving values
    void BookInventory::displayBookInformation(string title) {
        int index = findIndexNum(title);
        if (index == -1) {
            std::cout << "Book not found. " << std::endl;
        }
        else {
            std::cout << std::endl;
            std::cout << "ISBN: " << books[index].isbn << std::endl;
            std::cout << "Title: " << books[index].title << std::endl;
            std::cout << "Author: " << books[index].author << std::endl;
            std::cout << "Year of publication: " << books[index].year << std::endl;
            std::cout << "Publisher: " << books[index].publisher << std::endl;
            std::cout << "Genre: " << books[index].genre << std::endl;
            std::cout << "Description: " << books[index].description << std::endl;
            std::cout << "MSRP: $" << books[index].msrp << std::endl;
            std::cout << "Quantity: " << books[index].quantity << std::endl;
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

    // Gathers input for user list and returns it as map
    unordered_map<string, string> BookInventory::makeUserList() {
        // Declare variables
        unordered_map<string, string> userMap;
        string addISBN, addTitle, addAuthor, addYear, addPublisher, addGenre, addDescription, addMSRP, addQuantity;
        // Get values
        std::cout << "ISBN to add: " << std::endl;
        cin.ignore();
        std::getline(cin, addISBN);
        while (validISBNYear(addISBN, 13, 10) == false) {
            std::getline(cin, addISBN);
        }
        userMap["isbn"] = addISBN;
        std::cout << "Title to add: " << std::endl;
        std::getline(cin, addTitle);
        userMap["title"] = addTitle;
        std::cout << "Author to add: " << std::endl;
        std::getline(cin, addAuthor);
        userMap["author"] = addAuthor;
        std::cout << "Year to add: " << std::endl;
        std::getline(cin, addYear);
        while (validISBNYear(addYear, 4, 4) == false) {
            std::getline(cin, addYear);
        }
        userMap["year"] = addYear;
        std::cout << "Publisher to add: " << std::endl;
        std::getline(cin, addPublisher);
        userMap["publisher"] = addPublisher;
        std::cout << "Genre to add: " << std::endl;
        std::getline(cin, addGenre);
        userMap["genre"] = addGenre;
        std::cout << "Description to add: " << std::endl;
        std::getline(cin, addDescription);
        userMap["description"] = addDescription;
        std::cout << "MSRP to add: " << std::endl;
        std::getline(cin, addMSRP);
        while (validMSRP(addMSRP) == false) {
            std::getline(cin, addMSRP);
        }
        userMap["msrp"] = addMSRP;
        std::cout << "Quantity to add: " << std::endl;
        std::getline(cin, addQuantity);
        while (validQuantity(addQuantity) == false) {
            std::getline(cin, addQuantity);
        }
        userMap["quantity"] = addQuantity;
        return userMap;
    }

    // Inserts values from makeUserList to vector<Book> book
    void BookInventory::insertUserList(unordered_map<string, string> userMap, sqlite3 *db) {
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
        // Add data to vector<Book>
        books.push_back(newBook);
        // Add data to database
        addData(db, newBook.isbn.c_str(), newBook.title.c_str(), newBook.author.c_str(), newBook.year.c_str(), newBook.publisher.c_str(), newBook.genre.c_str(), newBook.description.c_str(), newBook.msrp, newBook.quantity);
    }

    // Write user to list to CSV
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
        file << std::endl;
        std::cout << "Entry successfully written to CSV file. " << std::endl;
        file.close();
    }

    // Display values of list user wishes to add
    void BookInventory::displayUserList(vector<string> userList) {
        for (int i = 0; i < userList.size(); i += 3) {
            std::cout << "ISBN: " << userList[i] << std::endl;
            std::cout << "Title: " << userList[i+1] << std::endl;
            std::cout << "Author: " << userList[i+2] << std::endl;
            std::cout << std::endl;
        }
        std::cout << "Total items: " << userList.size() / 3 << std::endl;
    }

    // Functor to sort vector<Book> by msrp value
    bool BookInventory::compareByValue(const Book& a, const Book& b) {
        return a.msrp < b.msrp;
    }

    // Write shopping list to CSV
    void BookInventory::writeList(vector<Book> list, string fileName) {
        // Open file
        file.open(fileName, ios_base::app);
        for (int i = 0; i < list.size(); i++) {
            file << list[i].isbn + ",";
            file << list[i].title + ",";
            file << list[i].author + ",";
            file << to_string(list[i].msrp) + ",";
            file << std::endl;
        }
        std::cout << std::endl; 
        std::cout << "Entry successfully written to CSV file. " << std::endl;
        file.close();
    }

    // Displays shopping list
    void BookInventory::displayList(vector<Book> list) {
        for (int i = 0; i < list.size(); i++) {
            std::cout << std::endl;
            std::cout << list[i].isbn << std::endl;
            std::cout << list[i].title << std::endl;
            std::cout << list[i].author << std::endl;
            std::cout << list[i].msrp << std::endl;
        }
    }

    // Validate ISBN and Year
    bool BookInventory::validISBNYear(string userTitle, int len1, int len2) {
        if ((userTitle.length() != len1) && (userTitle.length() != len2)) {
            std::cout << "Incorrect argument length. Please try again. " << std::endl;
            return false;
        }
        else {
            for (int i = 0; i < userTitle.length(); i++) {
                if (isdigit(userTitle[i]) == false) {
                    std::cout << "Number expected, but other character found. Please try again. " << std::endl; 
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
        // Check if valid using regex_match()
        if (regex_match(MSRP, integer_expr1) || regex_match(MSRP, integer_expr2) || regex_match(MSRP, integer_expr3)) {
            return true;
        }
        else {
            std::cout << "MSRPs must be written in the correct format, such as: 1.00, 10.00, or 100.00." << std::endl;
            return false;
        }
    }

    // Validate quantity
    bool BookInventory::validQuantity(string quant) {
        for (int i = 0; i < quant.length(); i++) {
            if (isdigit(quant[i]) == false) {
                std::cout << "Integer expected. Please try again. " << std::endl;
                return false;
            }
        }
        return true;
    }

    // Remove data from "books.db"
    int BookInventory::removeData(sqlite3 *db, const char *title) {
        char *zErrMsg = 0;
        int rc;
        sqlite3_stmt *stmt;
        const char *sql;
        
        sql = "DELETE FROM BOOKS WHERE TITLE = ?"; // writes sql statement to be executed
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            return rc;
        }
        rc = sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
            return rc;
        }
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
            return rc;
        }
        sqlite3_finalize(stmt);
        fprintf(stdout, "Data removed successfully from databse.\n");
        return SQLITE_OK;
    }

    // Add data to "books.db"
    int BookInventory::addData(sqlite3 *db, const char *isbn, const char *title, const char *author, const char *year, const char *publisher, const char *genre, const char *description, double msrp, int quantity) {
        // Use placeholders in the query string
        string query = "INSERT INTO BOOKS (ISBN, TITLE, AUTHOR, YEAR, PUBLISHER, GENRE, DESCRIPTION, MSRP, QUANTITY) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        sqlite3_stmt *stmt;

        // Prepare the statement
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
        }

        // Bind the parameters with the correct data types
        sqlite3_bind_text(stmt, 1, isbn, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, title, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, author, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, year, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, publisher, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, genre, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 7, description, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 8, msrp);
        sqlite3_bind_int(stmt, 9, quantity);

        // Execute the statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cout << "Data was not able to be added. " << std::endl;
            return -1;
        }
        std::cout << "Data successfully added to database. " << std::endl;

        // Finalize the statement
        sqlite3_finalize(stmt);
        return 0;
    }

    // Add new user to login.csv from admin menu
    void BookInventory::addNewUser(string username, string password) {
    	string encryptedPassword;
    	ofstream myFile;
    	// Open csv file
    	myFile.open(CSVfilename, ios_base::app);
    	// Exception handling
    	if(!myFile.is_open()){
    		cerr << "Failed to open file " << CSVfilename << endl;
    		return;
    	}
    	// Encrypt user entered password with md5
    	encryptedPassword = md5(password);
    	// Write username and password delimited by comma to login.csv
    	myFile << username << "," << encryptedPassword << endl;
    	myFile.close();
    	cout << "User: " << username << " added successfully." << endl;
    }

    // Change password of for existing account
    void BookInventory::changeUserPassword(string username, string password) {
    	string filename = "login.csv";
    	vector<User> users = readCSV(filename);

    	bool found = false;
    	for (int i = 0; i < users.size(); i++) {
    		if (users[i].username == username) {
    			cout << "Enter new password: ";
    			string password;
    			cin >> password;
    			users[i].password = md5(password);
    			found = true;
    			break;
    		}
    	}
    	if (!found) {
    		cout << "Incorrect username or password." << endl;
    	}
    	else {
    		writeCSV(filename, users);
    		cout << "Password changed." << endl;
    	}
    }

    // Add shopper to database
    int BookInventory::addShopper(sqlite3 *db, const char *name, const char *email) {
        // Use placeholders in the query string
        string query = "INSERT INTO SHOPPERS (NAME, EMAIL) VALUES (?, ?)";
        sqlite3_stmt *stmt;

        // Prepare the statement
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
        }

        // Bind the parameters with the correct data types
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cout << "Shopper was not able to be added. " << std::endl;
            return -1;
        }
        std::cout << "Shopper successfully added to database. " << std::endl;

        // Finalize the statement
        sqlite3_finalize(stmt);
        return 0;
    }

/*
    bool BookInventory::doesShopperExist(sqlite3* db, const char* email) {
        // Prepare the SQL statement with a parameter placeholder
        const char* sql = "SELECT * FROM SHOPPERS WHERE NAME = ?";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            return false;
        }

        // Bind the name parameter to the statement
        rc = sqlite3_bind_text(stmt, 1, email, -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }

        // Execute the statement and check if any rows were returned
        bool result = false;
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            result = true;
        } else if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        }

        // Finalize the statement and return the result
        sqlite3_finalize(stmt);
        return result;
    }
*/

    Receipt BookInventory::purchaseBooks(string title) {
        int amount = 0;
        int index = findIndexNum(title);
        Receipt receipt;
        if (index == -1) {
            std::cout << "Book not found. " << std::endl;
            return receipt;
        }
        else {
            std::cout << "Enter amount of books to be purchased: " << std::endl;
            cin >> amount;
            while ((books[index].quantity - amount) < 0) {
                std::cout << "Oops! We only have " << books[index].quantity << " copies available. Please enter a different amount." << std::endl;
                cin >> amount;
            }
            // Remove quantity from books
            books[index].quantity -= amount;
            // TO DO: Remove quantity from database

            // Add to receipt
            receipt.purchasedBook = title;
            receipt.amount = amount;
            receipt.msrp = books[index].msrp;
            double tax = books[index].msrp * amount * 0.06;
            receipt.tax = tax;
            receipt.total = books[index].msrp * amount + tax;
            return receipt;
        }
    }

    void BookInventory::displayReceipt(Receipt receipt) {
        std::cout << "Book: " << receipt.purchasedBook << std::endl;
        std::cout << "Quantity: " << receipt.amount << std::endl;
        std::cout << "MRSP: " << receipt.msrp << std::endl;
        std::cout << "Tax: " << receipt.tax << std::endl;
        std::cout << "Total: " << receipt.total << std::endl;
    }

    int BookInventory::addShopperTotal(sqlite3 *db, const char *name, double total) {
    // Prepare the SQL statement with parameter placeholders
        const char* sql = "UPDATE SHOPPERS SET TOTAL = ? WHERE NAME = ?";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            return rc;
        }

        // Bind the total and name parameters to the statement
        rc = sqlite3_bind_double(stmt, 1, total);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
        rc = sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }

        // Execute the statement and finalize it
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
        sqlite3_finalize(stmt);

        // Return the number of rows affected (should be 1)
        return sqlite3_changes(db);
    }

    // Coupon codes
    bool BookInventory::hasValidCoupon(string coupon) {
        coupon += ",";
        string line;
        ifstream file("coupon.csv");
        // Exception handling
        if (!file.is_open()) {
            cerr << "Failed to open file: coupon.csv" << endl;
        }
        // Search for coupon code
        while (getline(file, line)) {
            if (line == coupon) {
                return true;
            }
        }
        file.close();
        return false;
    }

#endif /* HEADER_H_ */