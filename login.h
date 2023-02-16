/*
 *  Filename: login.h
 *  Created on: Jan 29, 2023
 *  Author: Cameron Brooks, Julia Veenkamp
 */

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

#ifndef LOGIN_H_
#define LOGIN_H_
using namespace std;

class Login {
public:
    // Class constructor
    Login(int choice, string user, string pass) {
        loginNumber = choice;
        username = user;
        password = pass;

        if (loginNumber == 2) {
            createNewAccount(username, password);
        }
        else if (loginNumber == 1) {
            returningUserLogin(username, password);
        }
    }

private:
    string username, password, plainPassword, encryptedPassword;
    char ch;
    string user;
    string CSVfilename = "login.csv";
    ofstream myFile;
    int loginNumber;

    // Create new account
    void createNewAccount(string username, string password);

    // Verify returning user
    bool returningUserLogin(string username, string password);

};

    void Login::createNewAccount(string username, string password) {
	    // Open csv file
	    myFile.open(CSVfilename, ios_base::app);
	    // Exception handling
	    if(!myFile.is_open()){
		    cerr << "Failed to open file " << CSVfilename << endl;
		    return;
	    }
	    plainPassword = password;
	    for(int i =0; i < plainPassword.length(); i++) {
	    	ch = plainPassword.at(i);
	    	int a = (int) ch;
	    	string b = to_string(a);
	    	encryptedPassword.append(b);
	    }
	    // Write username and password delimited by comma
	    myFile << encryptedPassword << "," << encryptedPassword << endl;
	    myFile.close();

	    cout << "Thank you for creating an account with us, "  << username << "." << endl;
    }

    bool Login::returningUserLogin(string username, string password) {
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
            //Run input password through encryption
    	    for(int i =0; i < password.length(); i++) {
    	    	ch = password.at(i);
    	    	int a = (int) ch;
    	    	string b = to_string(a);
    	    	encryptedPassword.append(b);
    	    }
            if (username == storedUsername && encryptedPassword == storedPassword) {
                file.close();
                cout << "Welcome back, " << username << "." << endl;
                return true;
            }
        }
        file.close();
        cout << "Invalid username or password. Please try again later. " << endl;
        return false;
    }

#endif /* LOGIN_H_ */
