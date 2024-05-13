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


#define FIFO_FILE "myfifo"
#define SERVER_FILE "jobExecutorServer.txt"
#define SEM_NAME "/server_ready"


volatile sig_atomic_t signal_received = 0;
int Concurrency = 1;

void signal_handler(int signum) {
    if (signum == SIGCONT) {
        signal_received = 1;
    }
}

int main(int argc, char *argv[]) {
    queue_pointer pendingQueue = NULL;
    queue_pointer runningQueue = NULL;
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

        printf("im in token: %s\n", token);
        if (strcmp(token, "issueJob") == 0) {
            parameter = strtok(NULL, "");
            if (parameter == NULL)
                printf("\"issuejob\" : missing argument\n");
            else {
                printf("in else\n");
                issueJob(parameter, runningQueue, pendingQueue, job_id, 1);
                printf("after call\n");
                printf("Concurrency : %d\n", Concurrency);
                job_id++;
            }
        }

        if (strcmp(token, "setConcurrency") == 0) {
            printf("lalalalaaaaaaaaaaaaa\n");
			parameter = strtok(NULL, " ");
			Concurrency = atoi(parameter);
			/*function to update the queue */
            printf("%d\n", Concurrency);
			updated_Concurrency(runningQueue,pendingQueue);
		}








    }

    // Delete the SERVER_FILE
    if (unlink(SERVER_FILE) == -1) {
        perror("Failed to delete SERVER_FILE");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is Turning off\n");
    return 0;
}