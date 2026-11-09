#include <string.h>
#include <stdio.h>

#include "file.h"
#include "service.h"

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

        pid_t pid = start_service(service);

        if (pid > 0) {
            add_service(service, pid);
        }

    } else if (strcmp(action,"logs") == 0) {

    } else if (strcmp(action,"stop") == 0) {
        Service *tmpser = find_service(service);

        if (tmpser != NULL) {
            if (stop_service(tmpser->pid) == 0) {
                remove_service(service);
            }
        }
    }

    return "Test\n";
}
