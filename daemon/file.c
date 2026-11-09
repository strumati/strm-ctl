#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int check_file(const char *servicename)
{
    FILE *file = fopen(servicename, "r");

    if (file == NULL) {
        return 0;
    }

    fclose(file);
    return 1;
}

char *read_file(const char *servicename)
{
    FILE *file = fopen(servicename, "r");

    if (file == NULL) {
        return NULL;
    }

    char *content = malloc(4096);

    if (content == NULL) {
        fclose(file);
        return NULL;
    }

    if (fgets(content, 4096, file) == NULL) {
        free(content);
        fclose(file);
        return NULL;
    }

    fclose(file);

    return content;
}