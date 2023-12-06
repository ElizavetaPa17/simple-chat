#include "dbutility.h"

#include <stdio.h>
#include <stdlib.h>

const char *DB_PASSWD = "me7RKf13xZQ442";

DbUtility::DbUtility()
    : sql_handle_(nullptr)
{ 
}

DbUtility::~DbUtility() {
    closeConnection();
}

bool DbUtility::init()
{
    sql_handle_ = mysql_init(NULL);
    if (sql_handle_ == NULL) {
        closeConnectionWithError("D: Error to init database: ");
    } else {
        if (mysql_real_connect(sql_handle_, "localhost", "root", DB_PASSWD, NULL, 0, NULL, 0) == NULL) {
            closeConnectionWithError("D: Error to connect to the database server: ");
        } else {
            if (mysql_select_db(sql_handle_, "simple_chat_db") == 0) {
                fprintf(stdout, "D: SimpleChat database already exists.\n");
                return true;
            } else {
                fprintf(stderr, "D: SimpleChat database doesn't exist. Create new database...\n");
                if (mysql_query(sql_handle_, "CREATE DATABASE simple_chat_db")) {
                    closeConnectionWithError("D: Error to create the database: ");
                } else {
                    fprintf(stdout, "D: Successfull database creating. Using simple_chat_db as default database...\n");
                    if (mysql_query(sql_handle_, "USE simple_chat_db")) {
                        closeConnectionWithError("D: Error to use default database: ");
                    } else {
                        fprintf(stderr, "D: Successful using default database.\n"
                                        "D: Creating user table...\n");
                        if (mysql_query(sql_handle_, "CREATE TABLE users"
                                                     "(username VARCHAR(20), password VARCHAR(20))")) {
                            closeConnectionWithError("D: Error to create the user table: ");
                        } else {
                            fprintf(stderr, "D: Successful user creating.\n");
                            fprintf(stdout, "D: The database is ready to work.\n");
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool DbUtility::addUser(const char *username, const char *password) {
    static std::string buffer;

    buffer = std::string("INSERT INTO simple_chat_db VALUES ('") + username + "','" + password + "')";
    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to add user: %s\n", mysql_error(sql_handle_)); 
        return false;
    } 

    fprintf(stdout, "D: The user '%s' was added successfully.\n", username);
    return true;
}


void DbUtility::closeConnection() {
    mysql_close(sql_handle_);
}

void DbUtility::closeConnectionWithError(const char* message) {
    fprintf(stderr, "%s%s\n", message, mysql_error(sql_handle_));
    closeConnection();
}
