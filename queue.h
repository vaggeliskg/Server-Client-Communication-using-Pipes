// queue.c header file
#include <sys/types.h>

typedef struct node* queue_pointer;

typedef struct node {
    int job_id;
    char *job;
    pid_t pid;
    queue_pointer next;
} queue_node;


queue_pointer create_add_item(queue_pointer p, int job_id, char *job, pid_t pid);
int delete_item(queue_pointer *p, int job_id);
int count_items(queue_pointer p);
void print_queue(queue_pointer p); 
int get_first_id(queue_pointer p);
int queue_position(queue_pointer p, pid_t pid);
int return_id(queue_pointer p, pid_t pid);
