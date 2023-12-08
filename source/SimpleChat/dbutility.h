#ifndef DBUTILITY_H
#define DBUTILITY_H

#include <mysql/mysql.h>
#include <stddef.h>
#include <string>

class DbUtility final {
public:
    DbUtility();
    ~DbUtility();

    bool init();

    bool addUser(const char* username, const char* password);

    bool isUserExist(const char* username, const char* password);
    bool isUsernameExist(const char* username);
    
    //bool checkUserAuthentification;
    //bool checkUserMessages();

private:
    MYSQL* sql_handle_;
   
    bool setupUsersTable();
    bool setupMsgsTable();
    bool setupNewDatabase();

    void closeConnection();
    void closeConnectionWithError(const char* message);
};

#endif // DBUTILITY_H
