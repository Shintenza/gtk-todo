#include <bson/bson.h>
#include <mongoc.h>
#include "include/add_data.h"

void add_data ( mongoc_collection_t * collection, char task_name, char status, bool completed ) {
    bson_t *document;
    char *str;
    bson_oid_t oid;
    bson_error_t error;
    printf("hello there");

    document = BCON_NEW (
            "task_name:", BCON_UTF8(&task_name),
            "status", BCON_UTF8(&status),
            "urgent", BCON_BOOL(&completed)
        ); 
    bson_oid_init(&oid, NULL);
    BSON_APPEND_OID(document, "_id", &oid);

    if (!mongoc_collection_insert_one (
           collection, document, NULL, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }

    str = bson_as_canonical_extended_json(document, NULL);
   // printf("%s", str);
    bson_free(str);
    bson_destroy(document);
    mongoc_collection_destroy (collection);
}
