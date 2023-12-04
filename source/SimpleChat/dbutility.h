#ifndef DBUTILITY_H
#define DBUTILITY_H

#include <mysql/mysql.h>

class DbUtility
{
public:
    DbUtility();

private:
    MYSQL sql_handle_;
};

#endif // DBUTILITY_H
