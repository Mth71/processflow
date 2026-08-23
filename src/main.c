#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    if (argc > 2){
        fprintf(stderr, "uso: %s [workflowFile]\n", argv[0]);
        return 1;
    }

    if (argc == 2){
        printf("Modo WorkFlow. Arquivos recebidos: %s\n", argv[1]);
        
    }
    else{
        printf("Modo Interativo.\n");
    }
    return 0;
}