/*
*  Filename: main.cpp
*  Last edited on: 02/26/23
*  Authors: Veenkamp, Brooks, James
*/

#include <iostream>
#include <fstream>
#include <string>
#include "header.h"

using namespace std;
using json = nlohmann::json;

int main() {

    // Login
    int choice = 0;
    string username, password;
    std::cout << "Welcome to our book inventory program!" << std::endl;
    std::cout << "For returning users, enter 1. " << std::endl;
    std::cout << "For new users, enter 2. " << std::endl;
    cin >> choice;
    std::cout << "Please enter a username: " << std::endl;
    cin >> username;
    cout << username << endl;
    std::cout << "Please enter a password. " << std::endl;
    cin.ignore();
    cin >> password;
    cout << password << endl;
    Login log1(choice, username, password);

    // Main program
    int selection = 0;
    BookInventory book;
    book.printMenu();
    cin >> selection;
    while (selection != 1 && selection != 2 && selection != 3 && selection != 4 && selection != 5 && selection != 6) {
        cout << "Invalid selection. Please try again. " << endl;
        cin >> selection;
    }
    book.executeMenu(selection);
}