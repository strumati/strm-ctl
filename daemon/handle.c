#include <string.h>
#include <stdio.h>

#include "file.h"

char *handle(char *action,char *service)
{
    printf("action: %s\n",action);
    printf("service: %s\n",service);
    
    if (strcmp(action,"start") == 0) {
        printf("file lookup\n");

        char path[512];

        snprintf(path, sizeof(path), "./services/%s", service);

        if (check_file(path) == 0) {
            return "Error Service Not Found!\n";
        }

        char *content = read_file(path);

        if (content == NULL) {
            return "Error couldn't read Service\n";
        }

        printf("Service content: %s\n", content);


    } else if (strcmp(action,"lol") == 0) {

    }

    return "Test\n";
}
