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

    const UserInfo* getUserInfoByUsername(const char* username);
    const UserInfo* getUserInfoById(const char* id);
    std::vector<FetchedMessage> getAllMessages(const char* id, bool new_flag);
    std::vector<std::string> getAllSendersId(const char* to_id, bool new_flag);
    std::vector<FetchedMessage> getMessagesCertId(const char* to_id, const char* from_id);

private:
    MYSQL* sql_handle_;
   
    bool setupUsersTable();
    bool setupMsgsTable();
    bool setupNewDatabase();

    void closeConnection();
    void closeConnectionWithError(const char* message);

    const UserInfo* getUserInfo(const char* info, bool is_username);
};

#endif // DBUTILITY_H
