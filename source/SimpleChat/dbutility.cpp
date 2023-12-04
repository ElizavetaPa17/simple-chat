#include "dbutility.h"

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

DbUtility::DbUtility()
{
    printf("MySQL client version: %s\n", mysql_get_client_version());
}
