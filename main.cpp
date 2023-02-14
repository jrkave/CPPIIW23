// main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include "sqlite_table.h"
//#include "header.h" // take out when sqlite_table.h starts working
#include "login.h"

using namespace std;
using json = nlohmann::json;

int main() {

/*
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
    // Prompt for title
    string addTitle;
    cout << "Please input the title of your book: " << endl;
    cin.ignore();
    getline(cin, addTitle);
    cout << endl;
    book1.addBook(addTitle);
    // Remove added book
    book1.removeBook(addTitle);
  }

}

