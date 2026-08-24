#ifndef EXEC_H
#define EXEC_H

#include "task.h"

int run_single(Task*t);

int run_parallel(char**names,int n);

int run_pipeline(char**names, int n);

#endif