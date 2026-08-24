#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "exec.h"


int run_single(Task*t){
    pid_t pid =fork();

    if(pid<0){
        fprintf(stderr, "erro no fork(): '%s'\n", strerror(errno));
        return -1;
    }

    if(pid == 0){
        //processo filho

        if (t->input_file[0] != '\0'){
            int fd = open(t->input_file, O_RDONLY);
            if(fd < 0){
                fprintf(stderr, "erro ao abrir arquivo de entrada '%s': %s\n", t->input_file, strerror(errno));
                _exit(126);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if(t->output_file[0] != '\0'){
            int flags;
            if(t->append_mode){
                flags =  O_WRONLY | O_CREAT | O_APPEND;
            }else{
                flags = O_WRONLY | O_CREAT | O_TRUNC;
            }
            int fd = open(t->output_file, flags, 0644);
            if(fd < 0){
                fprintf(stderr, "erro ao abrir arquivo de saida '%s': %s\n", t->output_file, strerror(errno));
                _exit(126);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if (get_workdir()[0] != '\0') {
            if (chdir(get_workdir()) != 0) {
            fprintf(stderr, "erro ao mudar para o diretorio '%s': %s\n", get_workdir(), strerror(errno));
            _exit(126);
        }
        }
       
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


int run_parallel(char**names, int n){
    pid_t pids[MAX_TOKENS];
    int cont = 0;

    for(int i =0; i<n;i++){
        Task *t = find_task(names[i]);
        if(t == NULL){
            fprintf(stderr,"tarefa '%s' não encontrada\n", names[i]);
            continue;
        }

        pid_t pid = fork();
        if(pid < 0){
            fprintf(stderr, "erro no fork(): '%s'\n", strerror(errno));
            continue;
        }

        if (get_workdir()[0] != '\0') {
            if (chdir(get_workdir()) != 0) {
            fprintf(stderr, "erro ao mudar para o diretorio '%s': %s\n", get_workdir(), strerror(errno));
            _exit(126);
        }
        }

        if (pid == 0){
            execvp(t->program, t->argv);
            fprintf(stderr, "erro ao executar '%s': %s\n", t->program, strerror(errno));
            _exit(127);
        }

        pids[cont] = pid;
        cont++;
    }

    for(int i =0;i<cont;i++){
        int status;
        waitpid(pids[i], &status, 0);
    }
    return 0;
}

int run_pipeline(char**names, int n){
    Task *tasks[MAX_TOKENS];
    for(int i =0; i < n; i++){
        tasks[i] = find_task(names[i]);
        if(tasks[i] == NULL){
            fprintf(stderr, "tarefa '%s' não encontrada.\n", names[i]);
            return -1;
        }
    }

    int pipes[MAX_TOKENS][2];
    for(int i =0; i< n-1; i++){
        if(pipe(pipes[i]) != 0){
            fprintf(stderr, "erro ao criar pipe: '%s'\n", strerror(errno));
            return -1;
        }
    }

    pid_t pids[MAX_TOKENS];
    
    for(int i =0; i< n; i++){
        pid_t pid = fork();

        if(pid == 0){
            if(i > 0){
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            if(i < n - 1){
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for(int k =0 ; k<n-1;k++){
                close(pipes[k][0]);
                close(pipes[k][1]);
            }

            if (get_workdir()[0] != '\0') {
            if (chdir(get_workdir()) != 0) {
            fprintf(stderr, "erro ao mudar para o diretorio '%s': %s\n", get_workdir(), strerror(errno));
            _exit(126);
            }
            }
            execvp(tasks[i]->program, tasks[i]->argv);
            fprintf(stderr, "erro ao executar '%s': %s\n", tasks[i]->program, strerror(errno));
            _exit(127);
        }
        pids[i] = pid;
    }


    //pai fecha tds os pipelines(n precisa deles mais)

    for(int i = 0; i< n -1; i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    for(int i = 0; i<n; i++){
        int status;
        waitpid(pids[i], &status, 0);
    }
    return 0;
}

pid_t start_background(Task*t){
    pid_t pid =fork();

    if(pid<0){
        fprintf(stderr, "erro n fork(): %s\n", strerror(errno));
        return -1;
    }
        
    if (pid == 0){
        
        if (get_workdir()[0] != '\0') {
            if (chdir(get_workdir()) != 0) {
            fprintf(stderr, "erro ao mudar para o diretorio '%s': %s\n", get_workdir(), strerror(errno));
            _exit(126);
        }
        }
        execvp(t->program, t->argv);
        fprintf(stderr, "erro ao executar '%s: %s\n", t->program, strerror(errno));
        _exit(127);
    }
    return pid;
}