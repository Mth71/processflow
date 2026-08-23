#include <stdio.h>
#include <string.h>
#include "command.h"
#include "task.h"

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
    }else{
        printf("Comando desconhecido: '%s'\n", tokens[0]);
    }
    return 0;
}