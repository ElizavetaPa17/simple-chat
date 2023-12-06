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
    //bool checkUserAuthentification;
    //bool checkUserMessages();

private:
   MYSQL* sql_handle_;

   void closeConnection();
   void closeConnectionWithError(const char* message);
};

#endif // DBUTILITY_H
