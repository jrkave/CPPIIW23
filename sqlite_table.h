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

// Modify vectors prior to data insertion
    vector<string> avec = book.modifyVector(book.getAuthorsVec());
    vector<string> pvec = book.modifyVector(book.getPublisherVec());
    vector<string> tvec = book.modifyVector(book.getTitleVec());

// Insertion of data to db
int insert_data(sqlite3 *db) {
    char *zErrMsg = 0;
    int rc;
    sqlite3_stmt *stmt;
    const char *sql;

    sql = "INSERT INTO BOOKINVENTORY (ISBN,TITLE,AUTHOR,YEAR,PUBLISHER) " \
          "VALUES (?,?,?,?,?)";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Begin transaction
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &zErrMsg);

    int count = 2;
    for (int i = 1; i < book.getISBNVec().size(); i++) {
        if (i % 5427 == 0) {
            cout << count << "% complete. Continuing... " << endl;
            count += 2;
        }

        // Bind parameters to prepared statement
        sqlite3_bind_text(stmt, 1, book.getISBNVec()[i].c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, tvec[i].c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, avec[i].c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, book.getYearVec()[i].c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, pvec[i].c_str(), -1, SQLITE_TRANSIENT);

        // Execute prepared statement
        rc = sqlite3_step(stmt);
        if( rc != SQLITE_DONE ){
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            return rc;
        }

        // Reset prepared statement
        sqlite3_reset(stmt);
    }

    // Finalize prepared statement
    sqlite3_finalize(stmt);

    // Commit transaction
    sqlite3_exec(db, "COMMIT TRANSACTION;", NULL, NULL, &zErrMsg);

    fprintf(stdout, "Data inserted successfully\n");
    return 0;
};

