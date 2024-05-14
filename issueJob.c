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



// MIN KSEXASEIS FREE(JOB)

// global pointer
queue_pointer running_queue_global;
queue_pointer pending_queue_global;

void job_handler(int sig);
void split_command(char *command, char **args, int max_args);

void issueJob(char *command, queue_pointer running_queue, queue_pointer pending_queue, int job_id, int flag) {
    char *args[10];
    char answer[100];
    char *job = malloc(strlen(command) + 1); // Allocate memory for job
    if (job == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    pid_t pid = -1;
    printf("command is %s\n", command);
    //pid_t child_pid;
    strcpy(job, command); 
    printf("fsfssffs\n");
    printf("this is the job %s\n", job);
    split_command(command, args, 10);
    signal(SIGCHLD, job_handler);

    running_queue_global = running_queue;
    pending_queue_global = pending_queue;

    int number_of_running_proc = count_items(running_queue_global);
    if(number_of_running_proc < Concurrency) {
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
            printf("%d\n", Concurrency);
            //print_queue(&running_queue_global);
            create_add_item(&running_queue_global, job_id, job, pid);
            int pos = queue_position(running_queue_global,job_id);
            sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",job_id,"job: ", job,"Position: ",pos);
            send_answer(answer);
            // sprintf(answer, "%s","end");
            // send_answer(answer);
            
        }
    }
    else {
        create_add_item(&pending_queue_global, job_id, job, pid);
        // int pos = queue_position(pending_queue_global,job_id);
        // sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",job_id,"job: ", job,"Position: ",pos);
    }
}

// if Concurrency gets incremented add jobs
// if decremented do nothing just let the jobs finished and new Concurrency will take place
void updated_Concurrency(queue_pointer running_queue, queue_pointer pending_queue) {
    printf("hereeeeeee\n");
    int id2;
    char job[200];
    int number_of_running_processes;
    int number_of_pending_processes;

    running_queue_global = running_queue;
    pending_queue_global = pending_queue;

    printf("heresfsfsfsfsf\n");
    printf("concurrency isssss %d\n", Concurrency);
    number_of_running_processes = count_items(running_queue_global);

    //printf("concurrency isssss %d", Concurrency);
    if (number_of_running_processes < Concurrency) {
		for (int i=0 ; i<abs(Concurrency - number_of_running_processes) ; i++) {
            	/*as long as there are queued jobs*/
			number_of_pending_processes = count_items(pending_queue_global);
			if (number_of_pending_processes > 0) {
				//list = queued_jobs_list;
                id2 = get_first_id(pending_queue_global);
				strcpy(job, return_job(pending_queue_global,id2));
                		/*remove the oldest queued job*/
                delete_item(&pending_queue_global, id2);
				issueJob(job, running_queue_global, pending_queue_global, id2, 1);
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

    while ((pid = waitpid((pid_t)(-1), &var, WNOHANG)) > 0) {
        id = return_id(running_queue_global,pid);
        delete_item(&running_queue_global, id);

        //not finished
    }
    number_of_running_processes = count_items(running_queue_global);

    if (number_of_running_processes < Concurrency) {
		for (int i=0 ; i<abs(Concurrency - number_of_running_processes) ; i++) {
            	/*as long as there are queued jobs*/
			number_of_pending_processes = count_items(pending_queue_global);
			if (number_of_pending_processes > 0) {
				//list = queued_jobs_list;
                id2 = get_first_id(pending_queue_global);
				strcpy(job, return_job(pending_queue_global,id2));
                		/*remove the oldest queued job*/
                delete_item(&pending_queue_global, id2);
				issueJob(job, running_queue_global, pending_queue_global, id2, 1);
			}
		}
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