#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file.h"
#include "service.h"

void autostart()
{
    char *autocontent = read_file_full("ctlauto");

    if (autocontent != NULL) {
        char *saveptr;
        char *line = strtok_r(autocontent, "\n", &saveptr);

        while (line != NULL) {
            // trim
            while (isspace((unsigned char)*line))
                line++;

            // ignore # and empty lines
            if (*line != '#' && *line != '\0') {
                // trim
                char *end = line + strlen(line);
                while (end > line && isspace((unsigned char)end[-1]))
                    *--end = '\0';

                if (*line != '\0') {
                    printf("line: %s\n",line);

                    pid_t pid = start_service(line);

                    if (pid > 0) {
                        add_service(line, pid);
                    }
                }
            }

            line = strtok_r(NULL, "\n", &saveptr);
        }

        free(autocontent);
    }
}