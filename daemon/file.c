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

char *read_file_full(const char *servicename)
{
    FILE *file = fopen(servicename, "rb");

    if (file == NULL)
        return NULL;

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    long size = ftell(file);

    if (size < 0) {
        fclose(file);
        return NULL;
    }

    rewind(file);

    char *content = malloc((size_t)size + 1);

    if (content == NULL) {
        fclose(file);
        return NULL;
    }

    size_t read = fread(content, 1, (size_t)size, file);

    if (read != (size_t)size && ferror(file)) {
        free(content);
        fclose(file);
        return NULL;
    }

    content[read] = '\0';

    fclose(file);

    return content;
}