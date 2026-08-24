#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "command.h"
#include "task.h"
#include "exec.h"
#include "jobs.h"

static void handle_buscar(char**tokens, int ntokens){
    if(ntokens<2){
        printf("uso: buscar <nome da tarefa>\n");
        return;
    }

    Task *t = find_task(tokens[1]);
    if(t == NULL){
        printf("tarefa '%s' não encontrada.\n", tokens[1]);
        return;
    }

    printf("nome: %s\n", t->name);
    printf("programa: %s\n", t->program);
    printf("argc: %d\n", t->argc);

    for(int i =0; i<t->argc;i++){
        printf("argv[%d] = '%s'\n", i,t->argv[i]);
    }
}

static void handle_run(char**tokens, int ntokens){
    if(ntokens < 3){
        fprintf(stderr, "uso: run <sequential|parallel|pipe> <tarefa..>");
        return;
    }

    const char *mode = tokens[1];

    if(strcmp(mode,"sequential") == 0){
        for(int i = 2; i < ntokens; i++){
            Task*t = find_task(tokens[i]);
            if(t == NULL){
                fprintf(stderr, "tarefa '%s' não encontrada\n", tokens[i]);
                continue;
            }
            run_single(t);
        }
    }
    else if(strcmp(mode, "parallel") == 0){
        run_parallel(&tokens[2], ntokens - 2);
    }
    else if(strcmp(mode, "pipe") == 0){
        run_pipeline(&tokens[2], ntokens -2);
    }
    else{
        fprintf(stderr, "modo '%s' ainda não implementado\n", mode);

    }
}

static void handle_input(char**tokens, int ntokens){
    if(ntokens != 3){
        fprintf(stderr, "uso: input <tarefa> <arquivo>");
        return;
    }
    Task *t = find_task(tokens[1]);
    if(t == NULL){
        fprintf(stderr, "tarefa '%s' não encontrada\n", tokens[1]);
        return;
    }
    strncpy(t->input_file, tokens[2], MAX_LINE -1);
}

static void handle_output(char**tokens, int ntokens, int append){
    if(ntokens != 3){
        if(append){
            fprintf(stderr, "uso: append <tarefa> <arquivo>\n");
        }
        else{
            fprintf(stderr, "uso: output <tarefa> <arquivo>\n");
        }
        return;
    }
    Task*t = find_task(tokens[1]);
    if(t == NULL){
        fprintf(stderr, "tarefa '%s' não encontrada\n", tokens[1]);
        return;
    }
    strncpy(t->output_file, tokens[2], MAX_LINE -1);
    t->append_mode = append;
}

static void handle_start(char**tokens, int ntokens){
    if(ntokens != 2){
        fprintf(stderr, "uso: start <tarefa>");
        return;
    }

    Task*t = find_task(tokens[1]);
    if(t == NULL){
        fprintf(stderr, "tarefa '%s' não encontrada\n", tokens[1]);
        return;
    }
    pid_t pid = start_background(t);
    if(pid > 0){
        add_job(pid, tokens[1]);
    }
}

static void handle_wait(char**tokens, int ntokens){
    if(ntokens != 2){
        fprintf(stderr, "uso: wait <jobId>\n");
        return;
    }

    int id = atoi(tokens[1]);
    wait_job(id);
}


static void handle_workdir(char **tokens, int ntokens) {
    if (ntokens != 2) {
        fprintf(stderr, "uso: workdir <diretorio>\n");
        return;
    }
    struct stat sb;
    if (stat(tokens[1], &sb) != 0 || !S_ISDIR(sb.st_mode)) {
        fprintf(stderr, "erro: diretorio '%s' nao existe\n", tokens[1]);
        return;
    }
    set_workdir(tokens[1]);
    printf("diretorio de trabalho alterado para '%s'\n", tokens[1]);
}

int dispatch_command(char**tokens, int ntokens){
    if(ntokens == 0){
        return 0;
    }

    if(strcmp(tokens[0], "exit") == 0){
        return 1;
    }
    else if(strcmp(tokens[0], "task") == 0){
        add_task(tokens,ntokens);
    }
    else if(strcmp(tokens[0], "buscar") == 0){
        handle_buscar(tokens, ntokens);
    }
    else if(strcmp(tokens[0], "run") == 0){
        handle_run(tokens, ntokens);
    }
    else if(strcmp(tokens[0], "input")==0){
        handle_input(tokens, ntokens);
    }   
    else if(strcmp(tokens[0], "output") == 0){
        handle_output(tokens, ntokens, 0);
    }
    else if(strcmp(tokens [0], "append") == 0){
        handle_output(tokens, ntokens, 1);
    }
    else if(strcmp(tokens[0], "start") == 0){
        handle_start(tokens, ntokens);
    }
    else if(strcmp(tokens[0], "jobs") == 0){
        list_jobs();
    }
    else if(strcmp(tokens[0], "wait") == 0){
        handle_wait(tokens, ntokens);
    }
    else if(strcmp(tokens[0], "workdir") == 0){
        handle_workdir(tokens, ntokens);
    }
    else{
        printf("Comando desconhecido: '%s'\n", tokens[0]);
    }
    return 0;
}