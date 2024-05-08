#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXLEN 400
#define FIFO_FILE "jobCommanderToServer"
#define SERVER_FILE "jobExecutorServer.txt"


// void issueJob(char *job) {
   

//     int fd;
//     char buffer[256];

//     // Open the named pipe for writing
//     fd = open(FIFO_FILE, O_WRONLY);

//     // Write the issueJob command to the named pipe
//     sprintf(buffer, "issueJob %s", job);
//     write(fd, buffer, strlen(buffer) + 1);

//     // Close the named pipe
//     close(fd);
// }

// void setConcurrency(int N) {
    

//     int fd;
//     char buffer[256];

//     // Open the named pipe for writing
//     fd = open(FIFO_FILE, O_WRONLY);

//     // Write the setConcurrency command to the named pipe
//     sprintf(buffer, "setConcurrency %d", N);
//     write(fd, buffer, strlen(buffer) + 1);

//     // Close the named pipe
//     close(fd);
// }

// void stop(char *jobID) {


//     int fd;
//     char buffer[256];

//     // Open the named pipe for writing
//     fd = open(FIFO_FILE, O_WRONLY);

//     // Write the stop command to the named pipe
//     sprintf(buffer, "stop %s", jobID);
//     write(fd, buffer, strlen(buffer) + 1);

//     // Close the named pipe
//     close(fd);
// }

// void poll(char *type) {
   

//     int fd;
//     char buffer[256];

//     // Open the named pipe for writing
//     fd = open(FIFO_FILE, O_WRONLY);

//     // Write the poll command to the named pipe
//     sprintf(buffer, "poll %s", type);
//     write(fd, buffer, strlen(buffer) + 1);

//     // Close the named pipe
//     close(fd);
// }

// void exitServer() {
//     int fd;
//     char buffer[256];

//     // Open the named pipe for writing
//     fd = open(FIFO_FILE, O_WRONLY);

//     // Write the exit command to the named pipe
//     sprintf(buffer, "exit");
//     write(fd, buffer, strlen(buffer) + 1);

//     // Close the named pipe
//     close(fd);
// }


int isServerActive() {
    int fd = open(SERVER_FILE, O_RDONLY);
    if (fd != -1) {
        close(fd);
        return 1; // Server is active
    }
    return 0; // Server is not active
}



int main(int argc, char *argv[]) {
    if (isServerActive()) {
            printf("jobExecutorServer is active.\n");
    } else {
    // Start the server using a shell command
        int result = system("./jobExecutorServer");
    
        
        if (result == -1) {
            // system() failed to execute the command
            perror("Failed to start jobExecutorServer");
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for a brief moment to ensure the server has started
    sleep(1);
    

    // Create the named pipe
    if (mkfifo(FIFO_FILE, 0666) == -1) {
        perror("Error creating named pipe");
        exit(1);
    }

    printf("Named pipe created successfully\n");


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

    // int sf = open(SERVER_FILE, O_RDONLY);
    // if (sf == -1) {
    //     perror("Failed to open SERVER_FILE");
    //     exit(EXIT_FAILURE);
    // }

    // // Read the PID from the file
    // char pid_str[20]; // Assuming PID won't exceed 20 characters
    // ssize_t bytes_read = read(sf, pid_str, sizeof(pid_str));
    // if (bytes_read == -1) {
    //     perror("Failed to read PID from SERVER_FILE");
    //     close(sf);
    //     exit(EXIT_FAILURE);
    // }
    // close(sf);

    // // Convert the PID string to integer
    // pid_t server_pid = atoi(pid_str);
    // printf("Server PID: %d\n", server_pid);


    //kill(server_pid,SIGCONT);

    char buf[MAXLEN]; //Create a string containing all the arguments passed, to write in pipe
	buf[0] = '\0';
	for (int i = 1; i < argc - 1; i++)
	{
		strcat(buf, argv[i]);
		strcat(buf, " ");
	}
	strcat(buf, argv[argc - 1]);

    buf[strlen(buf) - 1] = '\0';
    printf("open\n");
    int fd = open(FIFO_FILE, O_WRONLY);
    if(fd == - 1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("2\n");
    int n = strlen(buf) + 1;
    printf("test1");
    if (write(fd, &n, sizeof(int))) {
        printf("1 error in write");
        return 3;
    }

    printf("test2");

    if (write(fd, buf, strlen(buf) + 1) < 0) {
        printf("error in write");
        return 3;
    }
    printf("written");
    close(fd);
    // if ((write(fd, buf, MAXLEN + 1)) == -1) //notify Server for command
	// 	{
	// 		perror(" Error in Writing ");
	// 		exit(2);
	// 	}
	// 	//kill(, SIGCONT); //send signal to Server that pipe has data

    return 0;
}