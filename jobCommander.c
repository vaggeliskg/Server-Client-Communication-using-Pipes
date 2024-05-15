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

#define MAXLEN 400
#define FIFO_FILE "myfifo"
#define SERVER_FILE "jobExecutorServer.txt"
#define SEM_NAME "/server_ready"
#define ANSWER_FILE "answer"



int isServerActive() {
    int fd2 = open(SERVER_FILE, O_RDONLY);
    if (fd2 != -1) {
        close(fd2);
        return 1; // Server is active
    }
    return 0; // Server is not active
}

int isPipeActive() {
    int fd3 = open(FIFO_FILE, O_RDONLY | O_NONBLOCK);
    if (fd3 != -1) {
        close(fd3);
        return 1; // Server is active
    }
    return 0; // Server is not active
}



int main(int argc, char *argv[]) {
    sem_t *sem;
    if (isServerActive()) {
            sem = sem_open(SEM_NAME, O_CREAT, 0666, 1); 
            if (sem == SEM_FAILED) {
                perror("sem_open");
                exit(EXIT_FAILURE);
            }
            if (sem_post(sem) == -1) {
                perror("sem_post");
                exit(EXIT_FAILURE);
            }
            printf("jobExecutorServer is active.\n");
    } else {
    // Start the server using a shell command
        sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
        if (sem == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
        int result = fork();
        if( result == 0 ) {
            execl("./jobExecutorServer", "./jobExecutorServer", NULL);
        }
        
        if (result == -1) {
            // system() failed to execute the command
            perror("Failed to start jobExecutorServer");
            exit(EXIT_FAILURE);
        }
    }
    //sleep(1);
      
    printf("Waiting for server...\n");
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
    printf("Server ready!\n");

    sem_close(sem);
    
    // create named pipe 

    //printf("im here\n");
    


    if(isPipeActive()) {
        printf("Named pipe '%s' already exists.\n", FIFO_FILE);
    }
    else {
        if (mkfifo(FIFO_FILE, 0666) == -1) {
                perror("Error creating named pipe");
                exit(EXIT_FAILURE);
            }
            printf("Named pipe '%s' created successfully.\n", FIFO_FILE);
    }

    if (argc < 2) {
        printf("Usage: %s <command> [arguments]\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "issueJob") == 0) {
        if (argc == 2) {
            printf("Usage: %s issueJob <job>\n", argv[0]);
            exit(1);
        }
        //issueJob(argv[2]);
    } else if (strcmp(argv[1], "setConcurrency") == 0) {
        if (argc != 3) {
            printf("Usage: %s setConcurrency <N>\n", argv[0]);
            exit(1);
        }
        //int N = atoi(argv[2]);
        //setConcurrency(N);
    } else if (strcmp(argv[1], "stop") == 0) {
        if (argc != 3) {
            printf("Usage: %s stop <jobID>\n", argv[0]);
            exit(1);
        }
        //stop(argv[2]);
    } else if (strcmp(argv[1], "poll") == 0) {
        if (argc != 3) {
            printf("Usage: %s poll [running|queued]\n", argv[0]);
            exit(1);
        }
        //poll(argv[2]);
    } else if (strcmp(argv[1], "exit") == 0) {
        if (argc != 2) {
            printf("Usage: %s exit\n", argv[0]);
            exit(1);
        }
        //exitServer();
    } else {
        printf("Unknown command: %s\n", argv[1]);
        exit(1);
    }

    // read server pid

    int sf = open(SERVER_FILE, O_RDONLY, S_IRUSR | S_IWUSR);
    if (sf == -1) {
        perror("Failed to open SERVER_FILE");
        exit(EXIT_FAILURE);
    }

    // Read the PID from the file
    char pid_str[20]; // Assuming PID won't exceed 20 characters
    ssize_t bytes_read = read(sf, pid_str, sizeof(pid_str));
    if (bytes_read == -1) {
        perror("Failed to read PID from SERVER_FILE");
        close(sf);
        exit(EXIT_FAILURE);
    }
    close(sf);

    // Convert the PID string to integer
    pid_t server_pid = atoi(pid_str);
    printf("Server PID: %d\n", server_pid);

    char buf[MAXLEN]; //Create a string containing all the arguments passed, to write in pipe
    buf[0] = '\0';
	for (int i = 1; i < argc - 1; i++)
	{
		strcat(buf, argv[i]);
		strcat(buf, " ");
	}
	strcat(buf, argv[argc - 1]);

    size_t len = strlen(buf);
    for( int i = 0; i < len; i ++) {
        if( buf[i] == '\n') {
            buf[i] = '\0';
            break;
        }
    }


    // // signal

    if (kill(server_pid, SIGCONT) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }
    printf("4444444444jfjafjajsfasfas\n");
    int fd = open(FIFO_FILE, O_WRONLY);
    if(fd == - 1) {
        perror("open\n");
        exit(EXIT_FAILURE);
    }

    int n = strlen(buf) + 1;
    printf("test1\n");
    if (write(fd, &n, sizeof(int)) < 0) {
        printf("1 error in write\n");
        return 3;
    }

    //printf("test2\n");

    if (write(fd, buf, strlen(buf) + 1) < 0) {
        printf("error in write");
        return 3;
    }
    printf("written\n");
    close(fd);


    while (1) {
    
    int fd7 = open(ANSWER_FILE, O_RDONLY);
    if (fd7 == -1) {
        perror("Failed to open SERVER_FILE 22222 in server");
        exit(EXIT_FAILURE);
    }
	int n = MAXLEN;
    char answer_buf[MAXLEN];
        //printf("in read\n");
        if(read(fd7, &n, sizeof(int)) < 0) {
            printf("end 1\n");
            return 6;
        }
        if(read(fd7, answer_buf, sizeof(char) * n) < 0) {
            printf("end 2\n");
            return 7;
        }

        size_t len = strlen(answer_buf);        
        for( int i = 0; i < len; i++) {
            printf("%c",answer_buf[i]);
        }

        printf("\n");
        // if (strcmp(answer_buf, "end") == 0) {
        //     printf("%s\n", answer_buf);
        break;
        // }

    // receive answer from server
    close(fd7);
    }

    return 0;
}