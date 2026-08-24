#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "exec.h"


int run_single(Task*t){
    pid_t pid =fork();

    if(pid<0){
        fprintf(stderr, "erro no fork(): '%s'\n", strerror(errno));
        return -1;
    }

    if(pid == 0){
        //processo filho
        execvp(t->program, t->argv);

        fprintf(stderr, "erro ao executar '%s': %s\n", t->program, strerror(errno));
        _exit(127);
    }

    int status;
    waitpid(pid, &status, 0);

    int exit_code;
    if(WIFEXITED(status)){
        exit_code = WEXITSTATUS(status);
    }else{
        exit_code = -1;
    }

    if (exit_code != 0){
        fprintf(stderr, "tarefa '%s' terminou com  código %d\n", t->name, exit_code);

    }

    return exit_code;

}