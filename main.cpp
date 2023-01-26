/* 
*   Filename: main.cpp
*   Date: Jan. 26, 2023
*   Author(s): Veenkamp, Brooks, James
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include "header.h"
using namespace std;

int main() {

// Example 1
    userInput = "Stardust";
    BookInformation(userInput);
    cout << endl;

// Example 2
    userInput = "0425061957";
    BookInformation(userInput);

// Prompts user for book title or ISBN number
    cout << "To view information about a book, please enter its title or ISBN: " << endl;
    cin >> userInput;
    BookInformation(userInput);

}
