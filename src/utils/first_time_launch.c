#include <stdio.h>
#include <sqlite3.h>
int first_time_launch (sqlite3 *db) {
    FILE *file;
    const char *db_file_name = "todo.db";
    if ((file = fopen(db_file_name, "r"))) {
        fclose(file);
        return 0;
    }
    return 1;
}
