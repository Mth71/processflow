#ifndef TASK_H
#define TASK_H

#include "common.h"

typedef struct TASK {
    char name[MAX_LINE];
    char program[MAX_LINE];
    char *argv[MAX_TOKENS];
    int argc;
}Task;

void init_tasks(void);
int add_task(char**tokens, int ntokens);
Task*find_task(const char *name);

#endif