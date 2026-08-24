#include <stdio.h>
#include <string.h>
#include "task.h"
#include "common.h"

static Task tasks[MAX_TASKS];
static int task_cont =0;
static char current_workdir[MAX_LINE] = "";

void init_tasks(void){
    task_cont =0;
} 

int add_task(char**tokens, int ntokens){
    if(ntokens<3){
        fprintf(stderr, "Erro! uso correto: task <nome> <programa> [argymentos..]\n");
        return -1;
    }

    if (task_cont >= MAX_TASKS){
        fprintf(stderr, "Erro! Número máximo de tarefas atingido.\n");
        return -1;
    }

    Task *t = &tasks[task_cont];

    strncpy(t->name, tokens[1], MAX_LINE - 1 );
    t->name[MAX_LINE - 1] = '\0';

    strncpy(t->program, tokens[2], MAX_LINE - 1 );
    t->program[MAX_LINE - 1] = '\0';

    int argc = 0;
    t->argv[argc] = t->program;
    argc++;

    for(int i = 3; i<ntokens && argc < MAX_TOKENS - 1; i++){
            t->argv[argc] = strdup(tokens[i])   ;
            argc++;
    }
    t->argv[argc] = NULL;
    t->argc = argc;

    t->input_file[0]='\0';
    t->output_file[0]='\0';
    t->append_mode = 0;

    task_cont++;
    printf("tarefa '%s' cadastrada (programa: '%s')\n", t->name,t->program);
    return 0;
}

Task *find_task(const char*name){
    for(int i =0;i<task_cont;i++){
        if(strcmp(tasks[i].name, name) == 0){
            return &tasks[i];
        }
    }
    return NULL;
}

void set_workdir(const char *dir) {
    strncpy(current_workdir, dir, MAX_LINE - 1);
}

const char *get_workdir(void) {
    return current_workdir;
}