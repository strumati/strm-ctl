#include <string.h>

#include "socket.h"

int main(int argc, char *args[]) {

    char buffer[256] = {0};

    for (int i = 0; i < argc; i++) {
        if (i > 0)
            strcat(buffer, " ");

        strcat(buffer, args[i]);
    }

    socket_send(SOCKET_PATH,buffer);
    return 0;
}
