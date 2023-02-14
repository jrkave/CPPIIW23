#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "header.h"
using namespace std;

BookInventory book;

int create_table(sqlite3 *db) {
    char *zErrMsg = 0;
    int rc;
    char sql[500];

    /* Create SQL statement */
    snprintf(sql, sizeof(sql), "CREATE TABLE IF NOT EXISTS BOOKINVENTORY("  \
                "ISBN TEXT PRIMARY KEY     NOT NULL,"\
                "TITLE           TEXT     NOT NULL," \
                "YEAR            TEXT     NOT NULL," \
                "AUTHOR          TEXT     NOT NULL," \
                "PUBLISHER       TEXT     NOT NULL);");

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return rc;
    } else {
        fprintf(stdout, "Table created successfully\n");  
    }
    return 0;
}

int insert_data(sqlite3 *db) {
    char *zErrMsg = 0;
    int rc;
    char sql[500];

    for (int i = 1; i < book.getTitleVec().size(); i++) {
        if (i % 1000 == 0) {
            cout << "1000 records inserted into database. Continuing... " << endl;
        }
        if (i == 67840) {
            cout << "25% complete." << endl;
        }
        // Handling apostrophes for author, publisher, title
        string title = book.getTitleVec()[i];
        string::iterator it = title.begin();
        while (it != title.end()) {
            if (*it == '\'') {
                it = title.insert(it, '\'');
                it++; // Advance past inserted character
            }
            it++; // Advance to next character
        }
        string author = book.getAuthorsVec()[i];
        it = author.begin();
        while (it != author.end()) {
            if (*it == '\'') {
                it = author.insert(it, '\'');
                it++;
            }
            it++;
        }
        string publisher = book.getPublisherVec()[i];
        it = publisher.begin();
        while (it != publisher.end()) {
            if (*it == '\'') {
                it = publisher.insert(it, '\'');
                it++;
            }
            it++;
        }
        // Create SQL statement
        snprintf(sql, sizeof(sql), "INSERT INTO BOOKINVENTORY (ISBN,TITLE,AUTHOR,YEAR,PUBLISHER) " \
                      "VALUES ('%s', '%s', '%s', '%s', '%s');",
                      book.getISBNVec()[i].c_str(), title.c_str(), author.c_str(), book.getYearVec()[i].c_str(), publisher.c_str());

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return rc;
        }
    }
    fprintf(stdout, "Data inserted successfully\n");
    return 0;
};
