// queue.c header file

typedef struct node* queue_pointer;

typedef struct node {
    int job_id;
    char *job;
    queue_pointer next;
} queue_node;


queue_pointer create_add_item(queue_pointer p, int job_id, char *job);
int delete_item(queue_pointer *p, int job_id);
int count_items(queue_pointer p);
void print_queue(queue_pointer p); 