#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

void db_error_handling (sqlite3 **db, char **err_msg) {
    fprintf(stderr, "Operation failed\n");
    fprintf(stderr, "SQL error: %s\n", *err_msg);

    sqlite3_free(*err_msg);
    sqlite3_close(*db);
    exit(1);
}
