#include "dbutility.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

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
                
                bool is_success = false;
                if (setupUsersTable()) {
                    fprintf(stdout, "D: 'users' table is ready to work.\n");
                    is_success = true;
                } else {
                    closeConnectionWithError("D: Failed to create the 'users' table: ");
                    is_success = false;
                }

                if (is_success && setupMsgsTable()) {
                    fprintf(stdout, "D: 'messages' table is ready to work.\n"
                                    "D: The database is ready to work.\n");
                } else {
                    closeConnectionWithError("D: Failed to create the 'messages' table: ");
                    is_success = false;
                }

                return is_success;
            } else {
                fprintf(stderr, "D: SimpleChat database doesn't exist. Create new database...\n");
                if (setupNewDatabase()) {
                    fprintf(stdout, "D: Successful 'users' creating.\n"
                                    "D: The database is ready to work.\n");
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    return false;
}

bool DbUtility::setupUsersTable() {
    if (mysql_query(sql_handle_, "SHOW TABLES LIKE 'users'")) {
        closeConnectionWithError("D: Can't determine 'users' table existance: ");
        return false;
    } else {
        bool is_success = false;
        MYSQL_RES* result;
        result = mysql_store_result(sql_handle_);
        if (result && result->row_count == 0) {
            fprintf(stdout, "D: The 'users' table doesn't exist. Creating new table...\n");
            if (mysql_query(sql_handle_, "CREATE TABLE users "
                                         "(id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,"
                                         "username VARCHAR(20) NOT NULL UNIQUE,"
                                         "password VARCHAR(20) NOT NULL,"
                                         "PRIMARY KEY (id))")) {
                closeConnectionWithError("D: Failed to create the 'users' table: ");
                is_success = false;
            } else {
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

bool DbUtility::setupMsgsTable() {
    if (mysql_query(sql_handle_, "SHOW TABLES LIKE 'messages'")) {
        closeConnectionWithError("D: Can't determine 'users' table existance: ");
        return false;
    } else {
        bool is_success = false;
        MYSQL_RES* result;
        result = mysql_store_result(sql_handle_);
        if (result && result->row_count == 0) {
            fprintf(stdout, "D: The 'messages' table doesn't exist. Creating new table...\n");
            if (mysql_query(sql_handle_, "CREATE TABLE messages "
                                         "(from_id SMALLINT UNSIGNED NOT NULL,"
                                         "to_id SMALLINT UNSIGNED NOT NULL,"
                                         "send_date DATETIME NOT NULL,"
                                         "is_new BOOL NOT NULL DEFAULT 1,"
                                         "msg_text TEXT NOT NULL,"
                                         "FOREIGN KEY (from_id) REFERENCES users (id) ON DELETE CASCADE,"
                                         "FOREIGN KEY (to_id) REFERENCES users (id) ON DELETE CASCADE)")) {
                closeConnectionWithError("D: Failed to create the 'users' table: ");
                is_success = false;
            } else {
                is_success = true;
            }
        } else if (result) {
            fprintf(stdout, "D: 'messages' table already exists.\n");
            is_success = true;
        } else {
            closeConnectionWithError("D: Can't determine 'messages' table existance: ");
            is_success = true;
        }

        mysql_free_result(result);
        return is_success;
    }
    return true;
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
            bool is_success = false;
            is_success = setupUsersTable();
            is_success &= setupMsgsTable();

            return is_success;
        }
    }
}

bool DbUtility::addUser(const char *username, const char *password) {
    static std::string buffer;

    if (isUsernameExist(username)) {
        fprintf(stderr, "D: Such a username already exists.\n");
        return false;
    }

    buffer = std::string("INSERT INTO users (username, password) VALUES"
                         "('") + username + "','" + password + "')";
    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to add user: %s\n", mysql_error(sql_handle_)); 
        return false;
    } 

    fprintf(stdout, "D: The user '%s' was added successfully.\n", username);
    return true;
}

bool DbUtility::addMessage(const char *from_id, const char *to_id, const char *date, const char *text) {
    static std::string buffer;
    static char escape_buffer[MAX_MSG_SIZE*2 + 1];
    
    mysql_escape_string(escape_buffer, text, strlen(text));
    buffer = std::string("INSERT INTO messages (from_id, to_id, send_date, msg_text) VALUES"
                         "('") + from_id + "','" + to_id + "','" + date + "','" + escape_buffer + "')";

    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to add the message: %s\n", mysql_error(sql_handle_));
        return false;
    }

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

const UserInfo* DbUtility::getUserInfoByUsername(const char *username) {
    return getUserInfo(username, true);
}

const UserInfo* DbUtility::getUserInfoById(const char *id) {
    return getUserInfo(id, false);
}

const UserInfo* DbUtility::getUserInfo(const char* info, bool is_username) {
    static std::string buffer;
    static struct UserInfo user_info;

    if (is_username) {
        buffer = std::string("SELECT * FROM users WHERE username='") + info + "'";
    } else {
        buffer = std::string("SELECT * FROM users WHERE id='") + info + "'";
    }

    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to check username existance: %s\n", mysql_error(sql_handle_));
        return NULL;
    } else {
        MYSQL_RES* result;
        result = mysql_store_result(sql_handle_);

        bool is_success = false;
        if (result) {
            MYSQL_ROW row;
            if ((row = mysql_fetch_row(result))) {
                size_t length = strlen(row[0]);
                if (length > ID_BUFFER_SIZE-1) {
                    fprintf(stderr, "D: Failed to get user information: the identificator is too long\n");
                    is_success = false;
                } else {
                    memcpy(user_info.id, row[0], length);
                    user_info.id[length] = '\0';
                
                    length = strlen(row[1]);
                    if (length > USRNM_BUFFER_SIZE-1) {
                        fprintf(stderr, "D: Failed to get user information: the username is too long\n");
                        is_success = false;
                    } else {
                        memcpy(user_info.username, row[1], length);
                        user_info.username[length] = '\0';
                        is_success = true;
                    }
                }

            } else {
                is_success = false;
            }
        } else if (mysql_field_count(sql_handle_)) {
            if (is_username) {
                fprintf(stderr, "D: Failed to find the username '%s': %s \n", info, mysql_error(sql_handle_));    
            } else {
                fprintf(stderr, "D: Failed to find the id '%s': %s \n", info, mysql_error(sql_handle_));    
            }
            is_success = false;    
        } 

        mysql_free_result(result); 
        if (is_success) {
            return &user_info;
        } else {
            return NULL;
        }
    }
}

std::vector<FetchedMessage> DbUtility::getAllMessages(const char *id, bool new_flag) {
    static std::string buffer;
    
    if (new_flag) {
        buffer = std::string("SELECT * FROM messages WHERE (to_id=\"") + id + "\" OR from_id=\"" + id + "\")" 
                             "AND is_new=\"1\" ORDER BY send_date";
    } else {
        buffer = std::string("SELECT * FROM messages WHERE to_id=\"") + id + "\"";
    }

    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to get new messages: %s\n", mysql_error(sql_handle_));
        return std::vector<FetchedMessage>(0);
    } else {
        MYSQL_RES* result = mysql_store_result(sql_handle_);
        std::vector<FetchedMessage> messages;

        if (result) {
            MYSQL_ROW row;      
            while ((row = mysql_fetch_row(result))) {
                messages.push_back(FetchedMessage{row[1], row[2], row[4]});
            }
        }

        mysql_free_result(result);
        return messages;
    }
}

std::vector<std::string> DbUtility::getAllSendersId(const char *to_id, bool new_flag) {
    static std::string buffer;
    
    
    if (new_flag) {
        buffer = std::string("SELECT DISTINCT from_id from messages where to_id=") + to_id + " AND is_new=1";
    } else {
        buffer = std::string("SELECT from_id from messages where to_id=") + to_id + " "
                             "UNION "
                             "SELECT to_id from messages where from_id=" + to_id;
    }

    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to get new messages: %s\n", mysql_error(sql_handle_));
        return std::vector<std::string>(0);
    } else {
        MYSQL_RES* result = mysql_store_result(sql_handle_);
        std::vector<std::string> messages;

        if (result) {
            MYSQL_ROW row;      
            while ((row = mysql_fetch_row(result))) {
                messages.push_back(std::string(row[0]));
            }
        }

        if (new_flag && mysql_query(sql_handle_, (std::string("UPDATE messages "
                                                  "SET is_new=0 "
                                                  "WHERE to_id=") + to_id + " AND is_new=1").c_str())) 
        {
            fprintf(stderr, "D: Failed to update new messages: %s\n", mysql_error(sql_handle_));
        }

        mysql_free_result(result);
        return messages;
    }
}

std::vector<FetchedMessage> DbUtility::getMessagesCertId(const char* to_id, const char* from_id)
{
    static std::string buffer;
    
    buffer = std::string("SELECT * FROM messages WHERE to_id=\"") + to_id + "\" AND from_id=\"" 
                         + from_id + "\" OR to_id=\"" + from_id + "\" AND from_id=\"" + to_id + "\""
                         "ORDER BY send_date";

    if (mysql_query(sql_handle_, buffer.c_str())) {
        fprintf(stderr, "D: Failed to get new messages: %s\n", mysql_error(sql_handle_));
        return std::vector<FetchedMessage>(0);
    } else {
        MYSQL_RES* result = mysql_store_result(sql_handle_);
        std::vector<FetchedMessage> messages;

        if (result) {
            MYSQL_ROW row;      
            while ((row = mysql_fetch_row(result))) {
                messages.push_back({row[0], row[1], row[2], row[4]});
            }
        }

        mysql_free_result(result);
        return messages;
    }
}


void DbUtility::closeConnection() {
    mysql_close(sql_handle_);
    sql_handle_ = NULL;
}

void DbUtility::closeConnectionWithError(const char* message) {
    fprintf(stderr, "%s%s\n", message, mysql_error(sql_handle_));
    closeConnection();
}
