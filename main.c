#include <stdio.h>
#include "Queue.h"

int main() {

    queue_t *head = malloc(sizeof(queue_t));
    head->count = 1;
    job_t *job = malloc(sizeof(job_t));
    head->job = job;
    job_t *curr = malloc(sizeof(job_t));
    head->add(head, curr);
//    printf("WTF?");
    printf("Count: %d", head->count);
    return 0;
}