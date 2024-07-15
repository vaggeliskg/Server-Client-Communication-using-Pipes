#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include "queue.h"
#include <sys/wait.h>



void job_handler(int sig);

void split_command(char *command, char **args, int max_args);

void issueJob(char *command, int job_id) {
    char *args[10];         
    char answer[100];
    char *job = malloc(strlen(command) + 1); // Allocate memory for job
    if (job == NULL) {
        perror("In malloc");
        exit(EXIT_FAILURE);
    }
    pid_t pid = -1;
    strcpy(job, command); //hold the command in job 

    split_command(command, args, 10);   // split the command for execution
    signal(SIGCHLD, job_handler);       // signal when a child has finished

    // if the it can fit in the running queue then start executing
    int number_of_running_proc = count_items(&runningQueue);
    if(number_of_running_proc < Concurrency) {
        if ((pid = fork()) < 0) { 
            perror("fork");
            exit(1);
        }
        if (pid == 0) { 
            // execute the command
            execvp(*args, args); 
            perror(*args);
            exit(1);
        }
        else {
            // if not in child then place in running queue
            create_add_item(&runningQueue, job_id, job, pid);
            int pos = queue_position(runningQueue,job_id);
            sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",job_id,"job: ", job,"Position: ",pos);
            send_answer(answer);
        }
    }
    else {
        // if it can't fit in running queue then place in pending queue
        create_add_item(&pendingQueue, job_id, job, pid);
        int pos = queue_position(pendingQueue,job_id);
        sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",job_id,"job: ", job,"Position: ",pos);
        send_answer(answer);
    }
}

// if Concurrency gets incremented add jobs
// if decremented do nothing just let the jobs finished and new Concurrency will take place
void updated_Concurrency() {

    int id2;
    char job[200];
    int number_of_running_processes;
    int number_of_pending_processes;

    number_of_running_processes = count_items(&runningQueue);

    if (number_of_running_processes < Concurrency) {
		for (int i=0 ; i<(Concurrency - number_of_running_processes) ; i++) {
			number_of_pending_processes = count_items(&pendingQueue);
			if (number_of_pending_processes > 0) {
                id2 = get_first_id(pendingQueue);
				strcpy(job, return_job(pendingQueue,id2));
                delete_item(&pendingQueue, id2);
				issueJob(job,id2);
			}
		}
	}
}

void job_handler(int sig) {
	pid_t pid;
    int var;
    int id;
    int id2;
    char job[200];
    int number_of_running_processes;
    int number_of_pending_processes;

    // detect childs that have finished executing
    while ((pid = waitpid((pid_t)(-1), &var, WNOHANG)) > 0) {
        id = return_id(runningQueue,pid);
        delete_item(&runningQueue, id);

    }
    // start getting jobs from pending queue and set them up for execution
    number_of_running_processes = count_items(&runningQueue);

    if (number_of_running_processes < Concurrency) {
		for (int i=0 ; i< (Concurrency - number_of_running_processes) ; i++) {
			number_of_pending_processes = count_items(&pendingQueue);
			if (number_of_pending_processes > 0) {
                id2 = get_first_id(pendingQueue);
				strcpy(job, return_job(pendingQueue,id2));
                delete_item(&pendingQueue, id2);
				issueJob(job, id2);
			}
		}
	}
}


// split the command string into args to execute it 
void split_command(char *command, char **args, int max_args) {
    char *token;
    int arg_count = 0;

    token = strtok(command, " ");
    
    while (token != NULL && arg_count < max_args - 1) {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;
}
