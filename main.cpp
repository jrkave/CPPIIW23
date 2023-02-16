// main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include "sqlite_table.h"
#include "login.h"

using namespace std;
using json = nlohmann::json;

int main() {

  // Open database
  sqlite3 *db;
  int rc;
  rc = sqlite3_open("test.db", &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return 1;
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  /* Commented out because test.db already exists and is populated
  // Create table and insert data
  create_table(db);
  insert_orig_data(db);
  */

  // Login functionality
  int input;
  string username, password;
  char promptChoice;

  cout << "Welcome to our book information program!" << endl;
  cout << "To get started, please enter 1 to login, or 2 to create an account. " << endl;
  cin >> input;
  // Validate input
  while (input != 1 && input != 2) {
    cout << "Invalid input. Please enter 1 to login, or 2 to create an account. " << endl;
    cin >> input;
  }
  
  cout << "Please enter a user name: " << endl;
  cin >> username;
  cout << "Please enter a password: " << endl;
  cin >> password;

  // Create instance of login class
  Login log1(input, username, password);

  // Retrieving book information
  cout << endl;
  cout << "Shortly, you will be prompted to input a book's title." << endl;
  cout << "If your book is in our system, we will return the following information: " << endl;
  cout << "ISBN, Title, Author, Year of Publication, and Publisher. " << endl;
  cout << endl;

  // User input
  string title;
  cout << "Please enter a book title: " << endl;
  cin.ignore();
  getline(cin, title);
  cout << "Retrieving book information... " << endl;
  cout << endl;

  // Creates instance of class using title
  BookInventory book1(title);
  book1.Display();

  // Continue prompt
  char quitProgram = ' ';
  while (quitProgram != 'q') {
    cout << "Would you like to stop entering books? Enter q for quit, or any other letter to continue. " << endl;
    cin >> quitProgram;
    if (quitProgram == 'q') {
      break;
    }
    else {
      cout << "Please enter a book title: " << endl;
      cin.ignore();
      getline(cin, title);
      cout << "Retrieving book information..." << endl;
      cout << endl;
      BookInventory book1(title);
      book1.Display();
      continue;
    }
  }

  // Adding books
  cout << endl;
  cout << "Would you like to add a book to the inventory? Enter y/n." << endl;
  cin >> promptChoice;
  while (promptChoice != 'y' && promptChoice != 'n') {
    cout << "Invalid entry. Please type 'y' for yes, 'n' for no. " << endl;
    cin >> promptChoice;
  }
  if (promptChoice == 'y') {
    // Prompts to direct user on adding data
    cout << "To add a book, we need to gather some additional information. " << endl;
    cout << "We will need your book's ISBN, Title, Author, Year of Publication, Publisher, Genre, and Description. " << endl;
    cout << "If you do not know a value, please type 'Unknown'." << endl;
    cout << "Additionally, please note that the ISBN and year entered (unless entering 'Unknown') must be 13-digit and 4-digit numbers, respectively." << endl;

    // Obtain user input
    string addISBN, addTitle, addAuthor, addYear, addPublisher, addGenre, addDescription;
    cin.ignore();
    cout << "Enter ISBN: " << endl;
    getline(cin, addISBN);
    // Validate ISBN
    if (addISBN != "Unknown" && addISBN != "unknown") {
      while (validInput(addISBN, 13) == false) {
        cin.clear();
        getline(cin, addISBN);
      }
    }
    cout << "Enter Title: " << endl;
    cin.clear();
    getline(cin, addTitle);
    cout << "Enter Author: " << endl;
    cin.clear();
    getline(cin, addAuthor);
    cout << "Enter Year of Publication: " << endl;
    cin.clear();
    getline(cin, addYear);
    // Validate year of publication
    if (addYear != "Unknown" && addYear != "unknown") {
      while (validInput(addYear, 4) == false) {
        cin.clear();
        getline(cin, addYear);
      }
    }
    cout << "Enter Publisher: " << endl;
    cin.clear();
    getline(cin, addPublisher);
    cout << "Enter Genre: " << endl;
    cin.clear();
    getline(cin, addGenre);
    cout << "Enter Description: " << endl;
    cin.clear();
    getline(cin, addDescription);

    // Add row in database
    add_data(db, addISBN.c_str(), addTitle.c_str(), addAuthor.c_str(), addYear.c_str(), addPublisher.c_str(), addGenre.c_str(), addDescription.c_str());
    // Add to vector in class
    book1.addToVec(addISBN, addTitle, addAuthor, addYear, addPublisher, addGenre, addDescription);
  }

  // Remove a book
  string delTitle;
  cout << endl;
  cout << "Would you like to remove a book to the inventory? Enter y/n." << endl;
  cin >> promptChoice;
  while (promptChoice != 'y' && promptChoice != 'n') {
    cout << "Invalid entry. Please type 'y' for yes, 'n' for no. " << endl;
    cin >> promptChoice;
  }
  if (promptChoice == 'y') {
    cout << "Please enter the title of the book you'd like to remove. " << endl;
    cin.ignore();
    getline(cin, delTitle);
    // Remove row from database
    remove_data(db, delTitle.c_str());
    // Remove element from vector
    book1.removeFromVec(delTitle);
  }

  // Make a list and display the list
  cout << "To make a list of books, please enter a title. We will check if it is in the database, and if it is, we will add it to your list. " << endl;
  book1.makeList();
  book1.DisplayList();
}
