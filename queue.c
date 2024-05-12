// Queue implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

// queue_pointer create_add_item(queue_pointer p, int job_id, char *job, pid_t pid) {
//     printf("heresfafasgasgs\n");
//     queue_pointer new_node = (queue_pointer)malloc(sizeof(queue_node));
//     if (new_node == NULL) {
//         perror("Failed to allocate memory for new node");
//         exit(EXIT_FAILURE);
//     }

//     new_node->job = (char*)malloc((strlen(job) + 1) * sizeof(char));
//     strcpy(new_node->job, job);
//     new_node->job_id = job_id;
//     new_node->pid = pid;
//     new_node->next = NULL;

//     // If the queue is empty, set the new node as the head
//     if (p == NULL) {
//         p = new_node;
//         printf("here\n");
//         return p;
//     }
//     else {
//     // Traverse the queue to find the last node
//     queue_pointer current = p;
//     printf("hereee\n");
//     while (current->next != NULL) {
//         current = current->next;
//     }

//     // Add the new node to the end of the queue
//     current->next = new_node;

//     return p; // Return the head of the queue
//     }
// }
void create_add_item(queue_pointer *p, int job_id, char *job, pid_t pid) {
    // Allocate memory for the new node
    queue_pointer new_node = (queue_pointer)malloc(sizeof(queue_node));
    if (new_node == NULL) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the job string and copy it
    new_node->job = (char*)malloc((strlen(job) + 1) * sizeof(char));
    if (new_node->job == NULL) {
        perror("Failed to allocate memory for job string");
        free(new_node);
        exit(EXIT_FAILURE);
    }
    strcpy(new_node->job, job);

    // Initialize other fields of the new node
    new_node->job_id = job_id;
    new_node->pid = pid;
    new_node->next = NULL;

    // If the queue is empty, set the new node as the head
    if (*p == NULL) {
        *p = new_node;
    } else {
        // Traverse the queue to find the last node
        queue_pointer current = *p;
        while (current->next != NULL) {
            current = current->next;
        }
        // Add the new node to the end of the queue
        current->next = new_node;
    }
}



int delete_item(queue_pointer *p, int job_id) {
    if (*p == NULL) {
        printf("Queue is empty. Cannot delete.\n");
        return 0;
    }

    queue_pointer current = *p;
    queue_pointer prev = NULL;

    // Traverse the queue to find the node with the specified job_id
    while (current != NULL && current->job_id != job_id) {
        prev = current;
        current = current->next;
    }

    // If the node with the specified job_id is not found
    if (current == NULL) {
        printf("Job with ID %d not found in the queue.\n", job_id);
        return 0;
    }

    // Update the pointers to remove the node
    if (prev == NULL) {
        *p = current->next;
    } else {
        prev->next = current->next;
    }

    // Free memory allocated for the job string and the node itself
    free(current->job);
    free(current);

    printf("Job with ID %d deleted from the queue.\n", job_id);
    return 1;
}


int count_items(queue_pointer p) {
    int count = 0;
    queue_pointer current = p;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void print_queue(queue_pointer *p) {
    if (*p == NULL) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue contents:\n");
    queue_pointer current = *p;
    while (current != NULL) {
        printf("Job ID: %d, Job: %s, pid: %d \n", current->job_id, current->job, current->pid);
        current = current->next;
    }
}


int get_first_id(queue_pointer p) {
    if (p == NULL) {
        printf("Queue is empty\n");
        return -1;
    } else {
        return p->job_id;
    }
}


int queue_position(queue_pointer p, pid_t pid) {
    int position = 1;
    queue_pointer current = p;

    // Traverse the queue
    while (current != NULL) {
        if (current->pid == pid) {
            return position;
        }
        position++;
        current = current->next;
    }

    // Job ID not found in the queue
    return -1;
}

int return_id(queue_pointer p, pid_t pid) {
    // Traverse the queue
    queue_pointer current = p;
    while (current != NULL) {
        // Check if the current node's PID matches the specified PID
        if (current->pid == pid) {
            // Return the job_id associated with this PID
            return current->job_id;
        }
        // Move to the next node
        current = current->next;
    }
    // If the PID is not found in the queue, return a special value to indicate failure
    return -1; // or any other suitable value to indicate failure
}
