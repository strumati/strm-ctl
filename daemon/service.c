#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    return 0;
}