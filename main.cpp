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
    std::cin >> choice;
    std::cout << "Please enter a username: " << std::endl;
    std::cin >> username;
    std::cout << "Please enter a password. " << std::endl;
    cin.ignore();
    cin >> password;
    Login log1(choice, username, password);

    // Main program
    int selection = 0;
    BookInventory book;
    book.printMenu();
    cin >> selection;
    while (selection < 1 && selection > 6) {
        cout << "Invalid selection. Please try again. " << endl;
        cin >> selection;
    }
    book.executeMenu(selection);
}