#include <stdio.h>  /* defines FILENAME_MAX */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define GetCurrentDir getcwd
void get_css_location(char *path) {
    char temp[FILENAME_MAX];
    char *temp_path = malloc(FILENAME_MAX);
    char *p;
    char *p2 = temp;
    char user_name[30];
    getlogin_r(user_name, sizeof(user_name));
    sprintf(temp_path, "/home/%s", user_name);
    getcwd(temp, sizeof(temp));
    p2+= strlen(p2)- 3;

    if(strcmp(p2, "bin") == 0) {
        p = temp;
        p[strlen(p) - 4] = '\0'; 
    }

    /*case when user decided to install this program*/
    if(strcmp(temp_path, temp)==0 ) {
        strcpy(path, "/opt/c_todo/style.css");
        free(temp_path);
        return;
    }

    strcat(temp, "/style.css");
    strcpy(path, temp);
}
