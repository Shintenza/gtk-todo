#ifndef ADD_DATA_H
#define ADD_DATA_H

#include <mongoc.h>
void add_data(mongoc_collection_t *collection, char task_name, char status, bool completed );
#endif
