#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "jobs.h"
#include "common.h"

static Job jobs[MAX_TASKS];
static int job_cont = 0;
static int next_id = 1;

void init_jobs(void){
    job_cont =0;
    next_id =1;   
}

int add_job(pid_t pid, const char*task_name){
    Job *j =&jobs[job_cont];
    j->id = next_id;
    j->pid = pid;
    strncpy(j->task_name, task_name, sizeof(j->task_name)-1);
    j->running =1;
    j->exit_code =0;

    job_cont++;
    int id = next_id;
    next_id++;

    printf("[%d] %d\n", id, pid);
    return id;
}

void poll_jobs(void){
    for(int i =0;i<job_cont;i++){
        if(jobs[i].running){
            int status;
            pid_t r = waitpid(jobs[i].pid, &status, WNOHANG);
            if(r == jobs[i].pid){
                jobs[i].running =0;
                if(WIFEXITED(status)){
                    jobs[i].exit_code = WEXITSTATUS(status);
                }else{
                    jobs[i].exit_code = -1;
                }
                printf("[%d] concluído (pid %d)\n", jobs[i].id, jobs[i].pid);
            }
        }
    }
}

void list_jobs(void){
    poll_jobs();
    if(job_cont == 0){
        printf("nenhum job iniciado\n");
        return;
    }
    for(int i =0; i< job_cont; i++){
        const char*status_str;
        if(jobs[i].running){
            status_str = "executando";
        }else{
            status_str = "concluído";
        }
        printf("[%d] pid=%d tarefa=%s status=%s\n", jobs[i].id, jobs[i].pid, jobs[i].task_name, status_str);
    }
}

int wait_job(int id){
    for(int i = 0; i<job_cont;i++){
        if(jobs[i].id == id){
            if(jobs[i].running){
                int status;
                waitpid(jobs[i].pid, &status, 0);
                jobs[i].running =0;
                if(WIFEXITED(status)){
                    jobs[i].exit_code = WEXITSTATUS(status);
                }else{
                    jobs[i].exit_code = -1;
                }
            }
            printf("job [%d] finalizado com codigo %d\n", id,jobs[i].exit_code);
            return jobs[i].exit_code;
        }
    }
    fprintf(stderr, "job %d não encontrado\n", id);
    return -1;
}