#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "header.h"
using namespace std;

BookInventory book;

// Creation of table in database
int create_table(sqlite3 *db) {
    char *zErrMsg = 0;
    int rc;
    char sql[500];

    // Create SQL statement for creating table in db
    snprintf(sql, sizeof(sql), "CREATE TABLE IF NOT EXISTS BOOKINVENTORY("  \
                "ISBN TEXT PRIMARY KEY     NOT NULL,"\
                "TITLE           TEXT     NOT NULL," \
                "YEAR            TEXT     NULL," \
                "AUTHOR          TEXT     NULL," \
                "PUBLISHER       TEXT     NULL);");

    // Execute SQL statement
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

// Modify vectors prior to data insertion since SQLite needs escape character '' for apostrophes
    vector<string> avec = book.modifyVector(book.getAuthorsVec());
    vector<string> pvec = book.modifyVector(book.getPublisherVec());
    vector<string> tvec = book.modifyVector(book.getTitleVec());

// Insertion of data to database from vectors obtained from json parsing
int insert_orig_data(sqlite3 *db) {
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

    int count = 10;
    for (int i = 1; i < 5001; i++) {
        if (i % 500 == 0) {
            if (i != 5000) {
                cout << count << "% complete. Continuing... " << endl;
                count += 10;
            }
            else {
                cout << "100% complete" << endl;
            }
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
}

// Remove data
int remove_data(sqlite3 *db, const char *title) {
    char *zErrMsg = 0;
    int rc;
    sqlite3_stmt *stmt;
    const char *sql;
    int count;

    // First, check if the title exists in the database
    sql = "SELECT COUNT(*) FROM BOOKINVENTORY WHERE TITLE = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    rc = sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    } else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    sqlite3_finalize(stmt);

    // If the title does not exist, return an error
    if (count == 0) {
        fprintf(stderr, "Entry not found\n");
        return -1;
    }

    // If the title exists, remove the data
    sql = "DELETE FROM BOOKINVENTORY WHERE TITLE = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    rc = sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    sqlite3_finalize(stmt);
    fprintf(stdout, "Data removed successfully\n");
    return SQLITE_OK;
}

// Add to database
int add_data(sqlite3 *db, const char *isbn, const char *title, const char *author, const char *year, const char *publisher, const char *genre, const char *description) {
    string query = "INSERT INTO BOOKINVENTORY (ISBN, TITLE, AUTHOR, YEAR, PUBLISHER, GENRE, DESCRIPTION) VALUES ('";
    query += string(isbn) + "', '" + string(title) + "', '" + string(author) + "', '" + string(year) + "', '" + string(publisher) + "', '" + string(genre) + "', '" + string(description) + "')";

    int rc = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);
    if (rc != SQLITE_OK) {
        return -1;
    }
    cout << "Data entered successfully" << endl;
    return 0;
}





