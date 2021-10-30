#include <mongoc.h>
#include "include/connect_db.h"

mongoc_client_t* db_connect ()
{
    const char *uri_string = "mongodb://localhost:27017";
    mongoc_uri_t *uri;
    mongoc_client_t *client;
    bson_error_t error;
     
    mongoc_init ();

    uri = mongoc_uri_new_with_error (uri_string, &error);
    if (!uri) {
       fprintf (stderr,
                "failed to parse URI: %s\n"
                "error message:       %s\n",
                uri_string,
                error.message);
    }
    
    client = mongoc_client_new_from_uri (uri);
    if (!client) {
    }
          
    mongoc_client_set_appname (client, "gtk_todo");
    mongoc_uri_destroy (uri);
    return client;
}
