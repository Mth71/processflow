#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

typedef struct {
    int id;
    pid_t pid;
    char task_name[128];
    int running;
    int exit_code;
} Job;

void init_jobs(void);
int add_job(pid_t pid, const char *task_name);
void list_jobs(void);
int wait_job(int id);
void poll_jobs(void);

#endif