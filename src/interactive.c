#include <stdio.h>
#include "interactive.h"
#include "common.h"
#include <string.h>

void run_interactive(void){
    char linha[MAX_LINE];

    while(1){
        printf("processflow> ");
        fflush(stdout);

        if (fgets(linha, sizeof(linha), stdin) == NULL){
            printf("\n");
            break;
        }

        linha[strcspn(linha, "\n")] = '\0';

        if(strcmp(linha, "exit") == 0){
            break;
        }

        printf("Você digitou: %s", linha);
    }
}