#include <stdio.h>
#include <stdlib.h>
#include "interactive.h"
#include "workflow.h" 

int main(int argc, char *argv[]){

    if (argc > 2){
        fprintf(stderr, "uso: %s [workflowFile]\n", argv[0]);
        return 1;
    }

    if (argc == 2){
        run_workflow(argv[1]);
    }
    else{
        run_interactive();
    }
    return 0;
}