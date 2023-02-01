/*
 *  Filename: LoginTest.h
 *  Created on: Jan 29, 2023
 *  Author: Cameron Brooks
 */

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

#ifndef LOGINTEST_H_
#define LOGINTEST_H_
using namespace std;

string username, password, nUsername, nPassword, loginChoice;
string CSVfilename = "login.csv";
ofstream myFile;
int loginNumber;

// Write new entry to CSV file
void createNewAccount(string file_name, string user_name, string pass_word) {
	// Open csv file
	myFile.open(CSVfilename, ios_base::app);
	// Exception handling
	if(!myFile.is_open()){
		cerr << "Failed to open file " << file_name << endl;
		return;
	}
	// Write username and password delimited by comma
	myFile << nUsername << "," << nPassword << endl;
	myFile.close();

	cout << "Thank you for creating an account with us." << endl;


}

// Search csv file for user information
bool returningUserLogin(string username, string password) {
    string line;
    ifstream file("login.csv");
    // Exception handling
    if (!file.is_open()) {
      cerr << "Failed to open file: login.csv" << endl;
      return false;
    }
    // Search csv file for username and password
    while (getline(file, line)) {
        int delimiterPos = line.find(',');
        string storedUsername = line.substr(0, delimiterPos);
        string storedPassword = line.substr(delimiterPos + 1);

        if (username == storedUsername && password == storedPassword) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Validate whether or not user input for login selection is number
bool validInput(string input) {

	for (int i = 0; i < input.length(); i++) {
		if (isdigit(input[i]) == false) {
			return false;
		}
	}
	if (input[0] == '0') {
		return false;
	}
	return true;
}

#endif /* LOGINTEST_H_ */