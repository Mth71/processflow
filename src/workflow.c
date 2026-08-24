#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workflow.h"
#include "common.h"
#include "parser.h"
#include "command.h"
#include "task.h"
#include "jobs.h"


void run_workflow(const char *filename){
    FILE *f = fopen(filename, "r");
    if(f == NULL){
        fprintf(stderr, "erro! não foi possível abrir o arquivo workflow '%s'\n", filename);
        exit(1);
    }

    init_tasks();
    init_jobs();

    char linha [MAX_LINE];
    char *tokens[MAX_TOKENS];
    int found_exit = 0;

    while(fgets(linha, sizeof(linha), f) != NULL){
        // cada linha deve ser impressa antes de ser processada
        printf("%s", linha);
        if(linha[strlen(linha)-1]!= '\n'){
            printf("\n");
        }

        linha[strcspn(linha, "\n")] = '\0';

        int n = tokenize(linha, tokens, MAX_TOKENS);

        int should_exit = dispatch_command(tokens, n);
        if(should_exit){
            found_exit = 1;
            break;
        }
    }

    fclose(f);
    if(!found_exit){
        fprintf(stderr, "aviso! arquivo workflow terminou sem o comando 'exit'\n");
    }
}