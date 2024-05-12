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

// global pointer
queue_pointer running_queue_global;

void job_handler(int sig);
void split_command(char *command, char **args, int max_args);

void issueJob(char *command, queue_pointer running_queue, queue_pointer pending_queue, int job_id, int flag) {
    char *args[10];
    char *job = malloc(strlen(command) + 1); // Allocate memory for job
    if (job == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    printf("command is %s\n", command);
    //pid_t child_pid;
    strcpy(job, command); 
    printf("fsfssffs\n");
    printf("this is the job %s\n", job);
    split_command(command, args, 10);
    signal(SIGCHLD, job_handler);

    running_queue_global = running_queue;

    if ((pid = fork()) < 0) { 
        perror("fork");
        exit(1);
    }
    if (pid == 0) { 
        /*execute the command*/
        execvp(*args, args); 
        perror(*args);
        exit(1);
        //child_pid = getpid();
    }
    else {
        printf("hiiiii\n");
        create_add_item(&running_queue_global, job_id, job, pid);
        print_queue(&running_queue_global);

    }

}


void job_handler(int sig) {
	pid_t pid;
    int var;
    int id;

    while ((pid = waitpid((pid_t)(-1), &var, WNOHANG)) > 0) {
        id = return_id(running_queue_global,pid);
        delete_item(&running_queue_global, id);

        //not finished
    }
}


void split_command(char *command, char **args, int max_args) {
    char *token;
    int arg_count = 0;

    // Tokenize the command string by whitespace
    token = strtok(command, " ");
    
    // Loop through each token and store it in args array
    while (token != NULL && arg_count < max_args - 1) {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " ");
    }
    
    // Null-terminate the args array
    args[arg_count] = NULL;
}