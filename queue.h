// queue.c header file
#include <sys/types.h>


typedef struct node* queue_pointer;
typedef char* string;

extern int Concurrency;
extern queue_pointer pendingQueue;
extern queue_pointer runningQueue;

typedef struct node {
    int job_id;
    char *job;
    pid_t pid;
    queue_pointer next;
} queue_node;


void create_add_item(queue_pointer *p, int job_id, char *job, pid_t pid);   // add item in the queue
int delete_item(queue_pointer *p, int job_id);                              // delete item from the queue
int count_items(queue_pointer *p);                                          // count the items in the queue
void print_queue(queue_pointer p);                                          // print queue
int get_first_id(queue_pointer p);                                          // get first id
int queue_position(queue_pointer p, int job_id);                            // return position of id in queue
int return_id(queue_pointer p, pid_t pid);                                  // return id 
string return_job(queue_pointer p, int job_id);                             // return job of id
int get_next_id(queue_pointer p, int current_id);                           // get the following id in queue


// funciton handling the jobs
void issueJob(char*, int);               

// function updating the queue based on concurrency
void updated_Concurrency();

// function to send the result back to the commander
void send_answer(string answer);
