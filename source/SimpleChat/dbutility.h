#ifndef DBUTILITY_H
#define DBUTILITY_H

#include <mysql/mysql.h>

class DbUtility
{
public:
    DbUtility();
    ~DbUtility();

    bool init();
    /*bool addUser();
    bool checkUserAuthentification;
    bool checkUserMessages();
    */

private:
   MYSQL* sql_handle_;

   void closeConnection();
   void closeConnectionWithError(const char* message);
};

#endif // DBUTILITY_H
