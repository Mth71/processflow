#include <stdio.h>
#include <string.h>
#include "interactive.h"
#include "common.h"
#include "parser.h"

#define MAX_TOKENS 64

void run_interactive(void){
    char linha[MAX_LINE];
    char *tokens[MAX_TOKENS];

    while(1){
        printf("processflow> ");
        fflush(stdout);

        if (fgets(linha, sizeof(linha), stdin) == NULL){
            printf("\n");
            break;
        }

        linha[strcspn(linha, "\n")] = '\0';

        int n = tokenize(linha, tokens, MAX_TOKENS);

        if(n == 0){
            continue;
        }

        if(strcmp(linha, "exit") == 0){
            break;
        }

        printf("Comando aceito: '%s' (com %d token(s) no total)\n", tokens[0], n);
    }
}