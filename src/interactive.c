#include <stdio.h>
#include <string.h>
#include "interactive.h"
#include "common.h"
#include "parser.h"
#include "task.h"
#include "command.h"


#define MAX_TOKENS 64

void run_interactive(void){
    char linha[MAX_LINE];
    char *tokens[MAX_TOKENS];

    init_tasks();

    while(1){
        printf("processflow> ");
        fflush(stdout);

        if(fgets(linha, sizeof(linha), stdin) == NULL){
            printf("/n");
            break;
        }

        linha[strcspn(linha, "\n")] = '\0';

        int n = tokenize(linha, tokens, MAX_TOKENS);

        int should_exist = dispatch_command(tokens, n);
        
        if(should_exist){
            break;
        }
    }
}