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
