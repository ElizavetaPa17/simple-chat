#include "dbutility.h"

#include <mysql/mysql.h>
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

bool DbUtility::init() {
    sql_handle_ = mysql_init(NULL);
    if (sql_handle_ == NULL) {
        closeConnectionWithError("D: Error to init database: ");
    } else {
        if (mysql_real_connect(sql_handle_, "localhost", "root", DB_PASSWD, NULL, 0, NULL, 0) == NULL) {
            closeConnectionWithError("D: Error to connect to the database server: ");
        } else {
            if (mysql_select_db(sql_handle_, "simple_chat_db") == 0) {
                fprintf(stdout, "D: SimpleChat database already exists. Checking table existance...\n");
                return checkUsersTableExistance();
            } else {
                fprintf(stderr, "D: SimpleChat database doesn't exist. Create new database...\n");
                return setupNewDatabase();
            }
        }
    }

    return false;
}

bool DbUtility::checkUsersTableExistance() {
    if (mysql_query(sql_handle_, "SHOW TABLES LIKE 'users'")) {
        closeConnectionWithError("D: Can't determine 'users' table existance: ");
        return false;
    } else {
        bool is_success = false;
        MYSQL_RES* result;
        result = mysql_store_result(sql_handle_);
        if (result && result->row_count == 0) {
            fprintf(stdout, "D: The 'users' table doesn't exist. Creating new table...\n");
            if (mysql_query(sql_handle_, "CREATE TABLE users"
                                         "(username VARCHAR(20), password VARCHAR(20))")) {
                closeConnectionWithError("D: Failed to create the 'user' table: ");
                mysql_free_result(result);
                is_success = false;
            } else {
                fprintf(stdout, "D: Successful 'users' table creating.\n"
                                "D: The database is ready to work.\n");
                is_success = true;
            }
        } else if (result) {
            fprintf(stdout, "D: 'users' table already exists.\n");
            is_success = true;
        } else {
            closeConnectionWithError("D: Can't determine 'users' table existance: ");
            is_success = true;
        }

        mysql_free_result(result);
        return is_success;
    }
}

bool DbUtility::setupNewDatabase() {
    if (mysql_query(sql_handle_, "CREATE DATABASE simple_chat_db")) {
        closeConnectionWithError("D: Error to create the database: "); 
        return false;
    } else {
        fprintf(stdout, "D: Successfull database creating. Using simple_chat_db as default database...\n");
        if (mysql_query(sql_handle_, "USE simple_chat_db")) {
            closeConnectionWithError("D: Error to use default database: ");
            return false;
        } else {
            fprintf(stderr, "D: Successful using default database.\n"
                            "D: Creating 'users' table...\n");
            if (mysql_query(sql_handle_, "CREATE TABLE users"
                                         "(username VARCHAR(20), password VARCHAR(20))")) {
                closeConnectionWithError("D: Error to create the user table: ");
                return false;
            } else {
                if (mysql_query(sql_handle_, "ALTER TABLE users ADD UNIQUE INDEX(username)")) {
                    fprintf(stderr, "Failed to setup database: %s\n", mysql_error(sql_handle_));
                    return false;
                }
                    fprintf(stdout, "D: Successful 'users' creating.\n"
                                    "D: The database is ready to work.\n");
                    return true;
            }
        }
    }
}

bool DbUtility::addUser(const char *username, const char *password) {
    static std::string buffer;

    if (isUsernameExist(username)) {
        fprintf(stderr, "D: Such a username already exists.\n");
        return false;
    }

    buffer = std::string("INSERT INTO users VALUES ('") + username + "','" + password + "')";
    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to add user: %s\n", mysql_error(sql_handle_)); 
        return false;
    } 

    fprintf(stdout, "D: The user '%s' was added successfully.\n", username);
    return true;
}

bool DbUtility::isUserExist(const char *username, const char *password) {
    static std::string buffer;
    buffer = std::string("SELECT * FROM users WHERE username = \"") + username + "\""
                         " AND password = \"" + password + "\"";

    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to check user existance: %s\n", mysql_error(sql_handle_));
        return true;
    } else {
       bool is_exist = false;
       MYSQL_RES* result; 
       result = mysql_store_result(sql_handle_);
       if (result && result->row_count) {
            is_exist = true;
       } else if (result) {
            is_exist = false;
       } else {
            fprintf(stderr, "D: Failed to check user existance: %s\n", mysql_error(sql_handle_));
            is_exist = true;
       }

       mysql_free_result(result);
       return is_exist;
    }
}

bool DbUtility::isUsernameExist(const char *username) {
    static std::string buffer;
    buffer = std::string("SELECT * FROM users WHERE username = '") + username + "'";
   
    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to check username existance: %s\n", mysql_error(sql_handle_));
        return true;
    } else {
       bool is_exist = false;
       MYSQL_RES* result; 
       result = mysql_store_result(sql_handle_);
       if (result && result->row_count) {
           is_exist = true;
       } else if (result) {
           is_exist = false;
       } else {
           fprintf(stderr, "D: Failed to check username existance: %s\n", mysql_error(sql_handle_));
           is_exist = true;
       }

       mysql_free_result(result);
       return is_exist;
    }
}

void DbUtility::closeConnection() {
    mysql_close(sql_handle_);
}

void DbUtility::closeConnectionWithError(const char* message) {
    fprintf(stderr, "%s%s\n", message, mysql_error(sql_handle_));
    closeConnection();
}
