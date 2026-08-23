#include <stdio.h>
#include "interactive.h"
#include "common.h"

void run_interactive(void){
    char linha[MAX_LINE];

    while(1){
        printf("processflow> ");
        fflush(stdout);

        if (fgets(linha, sizeof(linha), stdin) == NULL){
            printf("\n");
            break;
        }
        printf("Você digitou: %s", linha);
    }
}