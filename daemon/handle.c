#include <stdio.h>

char *handle(char *action,char *service)
{
    printf("action: %s\n",action);
    printf("service: %s\n",service);
    return "Test\n";
}
