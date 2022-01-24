#ifndef INITIAL_TASK_LOADING_H
#define INITIAL_TASK_LOADING_H 
#include "../global.h"
#include <sqlite3.h>
void init_load(sqlite3 *db, struct Task *tasks_arr, int *entries_counter, int *active_tasks_counter);
#endif
