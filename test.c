#include <stdio.h>
#include <stdlib.h>
#define MAX 10
struct CliTask {
    int rowid;
    char *name;
    long long unix_time;
};
void test(struct CliTask *test) {
    printf("%s", test[0].name);
}
int main() {
    char *random_string = "huja";
    char *second_string = "dupa";
    struct CliTask test_arr[10];
    test_arr[0].name = random_string;
    test(test_arr);
}
