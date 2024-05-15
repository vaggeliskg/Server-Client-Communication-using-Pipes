#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include "queue.h"

#define MAXLEN 400
#define FIFO_FILE "myfifo"
#define SERVER_FILE "jobExecutorServer.txt"
#define SEM_NAME "/server_ready"
#define ANSWER_FILE "answer"


volatile sig_atomic_t signal_received = 0;
int Concurrency = 1;
queue_pointer pendingQueue = NULL;
queue_pointer runningQueue = NULL;

int isAnswerActive() {
    int fd3 = open(ANSWER_FILE, O_RDONLY | O_NONBLOCK);
    if (fd3 != -1) {
        close(fd3);
        return 1; // answer is active
    }
    return 0; // answer is not active
}
void signal_handler(int signum) {
    if (signum == SIGCONT) {
        signal_received = 1;
    }
}

int main(int argc, char *argv[]) {
    // char answer_buf[MAXLEN];
    // char str[20];
    char answer[100];
    int job_id = 0;
    char* token;
    char* parameter;

    // Get the process ID (PID) of the server
    printf("Server is Turning on\n");
    pid_t server_pid = getpid();
    
    // Open the SERVER_FILE for writing
    int sf = open(SERVER_FILE, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (sf == -1) {
        perror("Failed to open SERVER_FILE in server");
        exit(EXIT_FAILURE);
    }

    // Convert the PID to a string
    char pid_str[20]; // Assuming PID won't exceed 20 characters
    snprintf(pid_str, sizeof(pid_str), "%d", server_pid);
    
    // Write the PID to the file
    write(sf, pid_str, strlen(pid_str));
    close(sf);

////////////////////////////////////////////////////////////////
    if(isAnswerActive()) {
        printf("Answer pipe '%s' already exists.\n", ANSWER_FILE);
    }
    else {
        if (mkfifo(ANSWER_FILE, 0666) == -1) {
                perror("Error creating named pipe");
                exit(EXIT_FAILURE);
            }
            printf("Answer pipe '%s' created successfully.\n", ANSWER_FILE);
    }

////////////////////////////////////////////////////////////////
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    printf("about to send\n");


    // Signal the semaphore to indicate that the server is ready
    if (sem_post(sem) == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
    printf("about to send\n");
    // Close the semaphore
    sem_close(sem);

    signal(SIGCONT,signal_handler);

    while(1) {
        while (signal_received == 0) {
            ;  // Wait for signal
        }
        signal_received = 0; // Reset the signal flag
        
        // read test
        int n;
        char buf[400];

        int fd = open(FIFO_FILE, O_RDONLY);
        if (fd == -1) {
            perror("Failed to open SERVER_FILE 22222 in server");
            exit(EXIT_FAILURE);
        }
        if(read(fd, &n, sizeof(int)) < 0) {
            printf("end 1\n");
            return 6;
        }
        if(read(fd, buf, sizeof(char) * n) < 0 ) {
            printf("end 2\n");
            return 7;
        }

        close(fd);
        size_t len = strlen(buf);
        for( int i = 0; i < len; i++) {
            printf("%c",buf[i]);
        }
        printf("\nread\n");

        // time for queue - forks etc


        token = strtok(buf, " ");

        if (strcmp(token, "issueJob") == 0) {
            parameter = strtok(NULL, "");
            if (parameter == NULL)
                printf("\"issuejob\" : missing argument\n");
            else {
                issueJob(parameter,job_id);
                job_id++;
                
            }
        }

        if(strcmp(token, "poll") == 0) {
            int another_id;
            string job = NULL;
            int position; //next_id;
            parameter = strtok(NULL," ");
            if (strcmp(parameter, "running") == 0) {
                int number = count_items(&runningQueue);
                print_queue(runningQueue);

                printf("queue has: %d\n",number);
                if(number > 0) {

                    another_id = get_first_id(runningQueue);
                    job = return_job(runningQueue, another_id);
                    position = queue_position(runningQueue, another_id);
                    sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",another_id,"job: ", job,"Position: ",position);
                    send_answer(answer);
                    //next_id = another_id;
                    
                    // for (int i = 0; i < number; i ++) {
                    //     next_id = get_next_id(runningQueue,next_id);
                    //     job = return_job(runningQueue, next_id);
                    //     position = queue_position(runningQueue, next_id);
                    //     sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",next_id,"job: ", job,"Position: ",position);
                    //     send_answer(answer);
                    //     // if( i == (number - 1) ) {
                    //     //     next_id = get_next_id(runningQueue,next_id);
                    //     //     job = return_job(runningQueue, next_id);
                    //     //     position = queue_position(runningQueue, next_id);
                    //     //     sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",next_id,"job: ", job,"Position: ",position);
                    //     //     send_answer(answer);
                    //     // }
                    // }

                    // sprintf(answer, "%s","end");
                    // send_answer(answer);
                }
                else {
                    sprintf(answer, "%s","Empty queue");
                    send_answer(answer);
                    // sprintf(answer, "%s","end");
                    // send_answer(answer);
                }
				// print_running(running_jobs_list, args);
				// send_response(args);
			}
			if (strcmp(parameter, "queued") == 0) {
				int number2 = count_items(&pendingQueue);
                printf("queue has: %d\n",number2);
                if(number2 > 0) {

                    another_id = get_first_id(pendingQueue);
                    job = return_job(pendingQueue, another_id);
                    position = queue_position(pendingQueue, another_id);
                    sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",another_id,"job: ", job,"Position: ",position);
                    send_answer(answer);
                    //next_id = another_id;
                    
                    // for (int i = 0; i < number2 ; i ++) {
                    //     next_id = get_next_id(pendingQueue,next_id);
                    //     job = return_job(pendingQueue, next_id);
                    //     position = queue_position(pendingQueue, next_id);
                    //     sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",next_id,"job: ", job,"Position: ",position);
                    //     send_answer(answer);
                    //     // if( i == (number - 1) ) {
                    //     //     // next_id = get_next_id(runningQueue,next_id);
                    //     //     // job = return_job(runningQueue, next_id);
                    //     //     // position = queue_position(runningQueue, next_id);
                    //     //     // sprintf(answer, "%s %d\t%s %s\t%s %d","job_id: ",next_id,"job: ", job,"Position: ",position);
                    //     //     // send_answer(answer);
                    //     // }
                    // }
                    // sprintf(answer, "%s","end");
                    // send_answer(answer);
			    }
                else {
                    sprintf(answer, "%s","Empty queue");
                    send_answer(answer);
                    // sprintf(answer, "%s","end");
                    // send_answer(answer);
                }
            }
        }

        if (strcmp(token, "setConcurrency") == 0) {
            
			parameter = strtok(NULL, " ");
			Concurrency = atoi(parameter);
			/*function to update the queue */
			updated_Concurrency();
            // might fix later using sem
            sprintf(answer, "%s %d", "Concurrency set to :", Concurrency);
			send_answer(answer);
            // sprintf(answer, "%s","end");
            // send_answer(answer);
		}

        if(strcmp(token, "stop") == 0) {
            parameter = strtok(NULL," ");
            int process_id = atoi(parameter);
            if(delete_item(&runningQueue,process_id)) {
                //found in running queue
                sprintf(answer,"%s %d", "Terminated job with id:",process_id);
                send_answer(answer);
                // sprintf(answer, "%s","end");
                // send_answer(answer);
            }
            else if(delete_item(&pendingQueue,process_id)) {
                sprintf(answer,"%s %d", "Removed job with id:",process_id);
                send_answer(answer);
                // sprintf(answer, "%s","end");
                // send_answer(answer);
            }
            else {
                sprintf(answer,"%s %d", "Did not find job with id:",process_id);
                send_answer(answer);
                // sprintf(answer, "%s","end");
                // send_answer(answer);
            }
        }

        if(strcmp(token, "exit") == 0) {
            // Delete the SERVER_FILE
            if (unlink(SERVER_FILE) == -1) {
                perror("Failed to delete SERVER_FILE");
                exit(EXIT_FAILURE);
            }
            
            sprintf(answer, "%s","jobExecutorServer terminated");
            send_answer(answer);
            // sprintf(answer, "%s","end");
            // send_answer(answer);
            return 0;
        }



    }

}



void send_answer(string answer) {
	int pipe;
    printf("in write\n");
	if( (pipe=open(ANSWER_FILE, O_WRONLY)) < 0) {
		perror("server writing: fifo open error");
		exit(1);
	}

    int n = strlen(answer) + 1;
    if (write(pipe, &n, sizeof(int)) < 0) {
        printf("1 error in write\n");
    }
    if (write(pipe, answer, strlen(answer) + 1) < 0) {
        printf("error in write");
    }
    printf("in server: %s\n", answer);

    // if(flag) {
    //     string end = "end";
    //     int x = strlen(end) + 1;

    //     if (write(pipe, &x, sizeof(int)) < 0) {
    //         printf("1 error in write\n");
    //     }
    //     end[0] = '\0';
    //     strcpy(end, "end");
    //     if ((write(pipe, end, strlen(end) + 1)) < 0) {
    //             perror("server : Error in Writing");
    //             exit(2);
    //     }
    //     printf("lalaloupsi\n");
    // }
	close(pipe);
}