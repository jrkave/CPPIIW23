// main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include "header.h"

using namespace std;
using json = nlohmann::json;

int main() {

    int selection = 0;
    BookInventory book;
    book.printMenu();
    cin >> selection;
    while (selection != 1 && selection != 2 && selection != 3) {
        cout << "Invalid selection. Please try again. " << endl;
        cin >> selection;
    }
    book.executeMenu(selection);
}