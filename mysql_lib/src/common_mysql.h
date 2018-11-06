#ifndef COMMON_MYSQL_H
#define COMMON_MYSQL_H

#ifdef AO_BUILD_LINUX
    #include <my_global.h>
    #include <mysql.h>
#endif

#include "../mysqlc/mysqlc/my_global.h"
#include "../mysqlc/mysqlc/mysql.h"

#endif // COMMON_MYSQL_H
