#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "autostart.h"
#include "handle.h"

#define SOCKET_PATH "/tmp/ctld.sock"
#define BUFFER_SIZE 1024

int main(void)
{
    autostart();

    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        unlink(SOCKET_PATH);
        return 1;
    }

    printf("Listening on %s\n", SOCKET_PATH);

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);

        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        ssize_t n;

        while ((n = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[n] = '\0';

            printf("Client: %s\n", buffer);

            char *args[32];
            int argc = 0;

            char *token = strtok(buffer, " ");

            while (token != NULL && argc < 32) {
                args[argc++] = token;
                token = strtok(NULL, " ");
            }

            int len = 0;

            if (argc > 2)
            {
                char *res = handle(args[1],args[2]);
                write(client_fd, res, strlen(res));
            } else {
                char *res = "ctl:\n - start <service>\n - logs <service>\n - stop <service>\n";
                write(client_fd, res, strlen(res));
            }
        }

        close(client_fd);
    }

    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
