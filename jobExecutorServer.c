#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>


#define FIFO_FILE "jobCommanderToServer"
#define SERVER_FILE "jobExecutorServer.txt"

int main(int argc, char *argv[]) {
    // Get the process ID (PID) of the server
    printf("Server is Turning on");
    pid_t server_pid = getpid();
    
    // Open the SERVER_FILE for writing
    int sf = open(SERVER_FILE, O_WRONLY | O_CREAT );
    if (sf == -1) {
        perror("Failed to open SERVER_FILE in server");
        exit(EXIT_FAILURE);
    }

    // Convert the PID to a string
    char pid_str[20]; // Assuming PID won't exceed 20 characters
    snprintf(pid_str, sizeof(pid_str), "%d", server_pid);
    
    // Write the PID to the file
    ssize_t bytes_written = write(sf, pid_str, strlen(pid_str));
    close(sf);


    // read test
    int n;
    char buf[400];
    printf("test1 in server");
    
    //pause();
    int fd = open(FIFO_FILE, O_RDONLY);
    if(read(fd, &n, sizeof(int)) < 0) {
        return 6;
    }
    printf("test2 in server");
    if(read(fd, buf, sizeof(char) * n) < 0 ) {
        return 7;
    }

    close(fd);
    printf("%s\n",buf);

    // Delete the SERVER_FILE
    if (unlink(SERVER_FILE) == -1) {
        perror("Failed to delete SERVER_FILE");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is Turning off");
    return 0;
}