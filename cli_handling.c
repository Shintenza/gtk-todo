#include "include/structs.h"
#include "include/messages.h"

int cli_handling (int argc, char **argv) {
    if (strcmp(argv[1], "--help")==0 || strcmp(argv[1], "-h")==0){
        printf("%s", help_message());
    }
    return 1;
}
