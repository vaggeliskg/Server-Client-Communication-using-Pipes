// Queue implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

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

    // Initialisation
    new_node->job_id = job_id;
    new_node->pid = pid;
    new_node->next = NULL;

    // If  empty, set the new node as the head
    if (*p == NULL) {
        *p = new_node;
    } else {
        // Find the last node
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

    // Find the id 
    while (current != NULL && current->job_id != job_id) {
        prev = current;
        current = current->next;
    }

    // If the node with the specified id is not found
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

    // Free allocated memory
    free(current->job);
    free(current);

    printf("Job with ID %d deleted from the queue.\n", job_id);
    return 1;
}


int count_items(queue_pointer *p) {
    int count = 0;
    queue_pointer current = *p;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void print_queue(queue_pointer p) {
    if (p == NULL) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue contents:\n");
    queue_pointer current = p;
    while (current != NULL) {
        printf("Job ID: %d, Job: %s, pid: %d, pos: %d \n", current->job_id, current->job, current->pid,queue_position(p, current->job_id) );
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


int queue_position(queue_pointer p, int job_id) {
    int position = 1;
    queue_pointer current = p;

    while (current != NULL) {
        if (current->job_id == job_id) {
            return position;
        }
        position++;
        current = current->next;
    }
    return -1;
}

int return_id(queue_pointer p, pid_t pid) {
    queue_pointer current = p;
    while (current != NULL) {
        if (current->pid == pid) {
            return current->job_id;
        }
        current = current->next;
    }
    return -1; 
}

string return_job(queue_pointer p, int job_id) {
    string str = "problem";
    queue_pointer current = p;
    while (current != NULL) {
        if (current->job_id == job_id) {
            return current->job;
        }
        current = current->next;
    }
    // If the PID is not found in the queue, return a special value to indicate failure
    printf("problem in returb_job\n");
    return str;
}

int get_next_id(queue_pointer p, int current_id) {
    queue_pointer current = p;
    int found = 0;
    int next_id = -1;
    while (current != NULL) {
        if (found) {
            next_id = current->job_id;
            break;
        }
        if (current->job_id == current_id) {
            found = 1;
        }
        current = current->next;
    }
    return next_id;
}