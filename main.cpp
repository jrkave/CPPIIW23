/* 
*   Filename: main.cpp
*   Date: Jan. 26, 2023
*   Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include "LoginTest.h"
#include "header.h"
using namespace std;

int main() {

    cout << "Welcome to our book information program!" << endl;
    cout << "Please enter 1 to login, or 2 to create an account." << endl;
	cin >> loginChoice;
	// Ensure that loginChoice is a number
	while(validInput(loginChoice) == false) {
		cout << "Invalid selection. Please enter 1 to login, or 2 to create an account." << endl;
		cin.clear();
		cin >> loginChoice;
		continue;
	}
	// Convert validated input to integer
	loginNumber = stoi(loginChoice);
	// Select login or new user
	while(loginNumber != 0){
		switch(loginNumber){
		// Returning user login
		case 1:
		    cout << "Enter username: ";
		    cin >> username;
		    cout << "Enter password: ";
		    cin >> password;
		    //Greet user on success, or allow them to make a new selection upon failure
		    if (returningUserLogin(username, password)) {
		        cout << "Welcome back, " << username << "." << endl;
		    }
		    else {
		        cout << "Incorrect username or password. Please enter 1 to login, or 2 to create an account." << endl;
		        cin.clear();
		        cin >> loginNumber;
		        break;
		    }
		    cin.clear();
		    loginNumber = 0;
		    break;
		// New user account creation
		case 2:
			cout << "Create a username: ";
			cin >> nUsername;
			cout << "Create a password: ";
			cin >> nPassword;
			// Write new user info to login.csv
			createNewAccount(CSVfilename, nUsername, nPassword);
			cin.clear();
			loginNumber = 0;
			break;
		// If user did not enter 1 or 2
		default:
			cout << "Invalid selection. Please enter 1 to login, or 2 to create an account." << std::endl;
			cin.clear();
			loginNumber = 0;
			cin >> loginNumber;
			break;
		}
	}

    // Welcome Part 2 Message
    cout << endl;
    cout << "Shortly, you will be prompted to input a book's title or ISBN." << endl;
    cout << "If your book is in our system, we will return the following information: " << endl;
    cout << "ISBN, Title, Author, Year of Publication, and Publisher. " << endl;
    cout << endl;

    // User input
    string input;
    cout << "Please enter a book's title or ISBN number: " << endl;
    cin >> input;
    cout << "Retrieving book information..." << endl;
    cout << endl;

    // Create instance of class BookInformation using input
    BookInformation book1(input);
    book1.Display();

    // Continue prompt
    char quitProgram = ' ';
    while (quitProgram != 'q') {
        cout << "Would you like to quit? Enter q for quit, and any other letter to continue. " << endl;
        cin >> quitProgram;
        if (quitProgram == 'q') {
            break;
        }
        else {
            cout << "Please enter a book's title or ISBN number: " << endl;
            cin >> input;
            cout << "Retrieving book information..." << endl;
            cout << endl;
            BookInformation book1(input);
            book1.Display();
        }
    }
};