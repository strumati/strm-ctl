#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "file.h"
#include "service.h"

Service services[MAX_SERVICES];
int service_count = 0;

pid_t start_service(const char *service)
{
    char path[512];
    char logpath[512];

    snprintf(path, sizeof(path), "./services/%s", service);
    snprintf(logpath, sizeof(logpath), "./logs/%s.log", service);

    char *command = read_file(path);

    if (command == NULL) {
        return -1;
    }

    command[strcspn(command, "\r\n")] = '\0';

    pid_t pid = fork();

    if (pid < 0) {
        free(command);
        return -1;
    }

    if (pid == 0) {
        char working_dir[512];

        strncpy(working_dir, path, sizeof(working_dir));
        working_dir[sizeof(working_dir) - 1] = '\0';

        printf("command: %s",command);
        printf("working_dir: %s",working_dir);

        if (chdir(working_dir) != 0) {
            _exit(1);
        }

        int log = open(logpath, O_WRONLY | O_CREAT | O_APPEND, 0644);

        if (log < 0) {
            _exit(1);
        }

        dup2(log, STDOUT_FILENO);
        dup2(log, STDERR_FILENO);

        close(log);

        char *argv[] = { command, NULL };

        execv(command, argv);

        _exit(127);
    }

    free(command);

    return pid;
}

int stop_service(pid_t pid)
{
    if (kill(pid, SIGTERM) == -1) {
        return -1;
    }

    int status;

    while (waitpid(pid, &status, 0) == -1) {
        if (errno != EINTR) {
            return -1;
        }
    }

    return 0;
}

Service *find_service(const char *name)
{
    for (int i = 0; i < service_count; i++) {
        if (strcmp(services[i].name, name) == 0) {
            return &services[i];
        }
    }

    return NULL;
}


int add_service(const char *name, pid_t pid)
{
    if (service_count >= MAX_SERVICES)
        return -1;

    if (find_service(name) != NULL)
        return -1;

    snprintf(services[service_count].name, sizeof(services[service_count].name), "%s", name);

    services[service_count].pid = pid;

    service_count++;

    return 0;
}

int remove_service(const char *name)
{
    for (int i = 0; i < service_count; i++) {
        if (strcmp(services[i].name, name) == 0) {

            for (int j = i; j < service_count - 1; j++) {
                services[j] = services[j + 1];
            }

            service_count--;

            return 0;
        }
    }

    return -1;
}