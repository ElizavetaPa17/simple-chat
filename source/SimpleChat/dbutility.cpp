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
        closeConnectionWithError("Error to init database: ");
    } else {
        if (mysql_real_connect(sql_handle_, "localhost", "root", DB_PASSWD, NULL, 0, NULL, 0) == NULL) {
            closeConnectionWithError("Error to connect to the database server: ");
        } else {
            if (mysql_select_db(sql_handle_, "simple_chat_db") == 0) {
                fprintf(stdout, "SimpleChat database already exists.\n");
                return true;
            } else {
                fprintf(stderr, "SimpleChat database doesn't exist. Create new database...\n");
                if (mysql_query(sql_handle_, "CREATE DATABASE simple_chat_db")) {
                    closeConnectionWithError("Error to create the database: ");
                } else {
                    fprintf(stdout, "Successful database creating. Using simple_chat_db as default database...\n");
                    if (mysql_query(sql_handle_, "USE simple_chat_db")) {
                        closeConnectionWithError("Error to use default database: ");
                    } else {
                        fprintf(stderr, "Successful using default database.\n"
                                        "Creating user table...\n");
                        if (mysql_query(sql_handle_, "CREATE TABLE users"
                                                     "(username VARCHAR(20), password VARCHAR(20))")) {
                            closeConnectionWithError("Error to create the user table: ");
                        } else {
                            fprintf(stderr, "Successful user creating.\n");
                            fprintf(stdout, "The database is ready to work.\n");
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}


void DbUtility::closeConnection() {
    mysql_close(sql_handle_);
}

void DbUtility::closeConnectionWithError(const char* message) {
    fprintf(stderr, "%s%s\n", message, mysql_error(sql_handle_));
    closeConnection();
}
