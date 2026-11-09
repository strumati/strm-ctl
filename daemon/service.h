#ifndef SERVICE_H
#define SERVICE_H

#include <sys/types.h>

#define MAX_SERVICES 128

typedef struct {
    char name[128];
    pid_t pid;
} Service;

extern Service services[MAX_SERVICES];
extern int service_count;

pid_t start_service(const char *service);
int stop_service(pid_t pid);

int add_service(const char *name, pid_t pid);
int remove_service(const char *name);
Service *find_service(const char *name);
void list_services(void);

#endif