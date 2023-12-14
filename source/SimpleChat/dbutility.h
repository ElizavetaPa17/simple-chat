#ifndef DBUTILITY_H
#define DBUTILITY_H

#include <mysql/mysql.h>
#include <stddef.h>
#include <string>
#include <vector>

#include "constants.h"

class DbUtility final {
public:
    DbUtility();
    ~DbUtility();

    bool init();

    bool addUser(const char* username, const char* password);
    bool addMessage(const char* from_id, const char* to_id, const char* date, const char* text);

    bool isUserExist(const char* username, const char* password);
    bool isUsernameExist(const char* username);

    const UserInfo* getUserInfo(const char* username);
    std::vector<FetchedMessage> getAllNewMessages(const char* id);

private:
    MYSQL* sql_handle_;
   
    bool setupUsersTable();
    bool setupMsgsTable();
    bool setupNewDatabase();

    void closeConnection();
    void closeConnectionWithError(const char* message);
};

#endif // DBUTILITY_H
