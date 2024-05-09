// Queue implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

queue_pointer create_add_item(queue_pointer p, int job_id, char *job) {

    queue_pointer new_node = (queue_pointer)malloc(sizeof(queue_node));
    if (new_node == NULL) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    new_node->job = (char*)malloc((strlen(job) + 1) * sizeof(char));
    strcpy(new_node->job, job);
    new_node->job_id = job_id;
    new_node->next = NULL;

    // If the queue is empty, set the new node as the head
    if (p == NULL) {
        return new_node;
    }
    else {
    // Traverse the queue to find the last node
    queue_pointer current = p;

    while (current->next != NULL) {
        current = current->next;
    }

    // Add the new node to the end of the queue
    current->next = new_node;

    return p; // Return the head of the queue
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

void print_queue(queue_pointer p) {
    if (p == NULL) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue contents:\n");
    queue_pointer current = p;
    while (current != NULL) {
        printf("Job ID: %d, Job: %s\n", current->job_id, current->job);
        current = current->next;
    }
}
