#ifndef DB_ERROR_H
#define DB_ERROR_H
#include <sqlite3.h>
void db_error_handling (sqlite3 **db, char **err_msg);
#endif
