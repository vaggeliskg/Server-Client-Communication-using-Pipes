// queue.c header file
#include <sys/types.h>


typedef struct node* queue_pointer;
extern int Concurrency;
typedef char* string;

extern queue_pointer pendingQueue;
extern queue_pointer runningQueue;

typedef struct node {
    int job_id;
    char *job;
    pid_t pid;
    queue_pointer next;
} queue_node;


void create_add_item(queue_pointer *p, int job_id, char *job, pid_t pid);
int delete_item(queue_pointer *p, int job_id);
int count_items(queue_pointer *p);
void print_queue(queue_pointer p); 
int get_first_id(queue_pointer p);
int queue_position(queue_pointer p, int job_id);
int return_id(queue_pointer p, pid_t pid);
string return_job(queue_pointer p, int job_id);
int get_next_id(queue_pointer p, int current_id);

void issueJob(char*, int);
void updated_Concurrency();
void send_answer(string answer);
