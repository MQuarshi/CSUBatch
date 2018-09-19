#include <stdio.h>
#include "Queue.h"

int main() {

    queue_t *head = init_queue(head);
    job_t *job = malloc(sizeof(job_t));;
    head->add(head, job);
    job_t *job1 = malloc(sizeof(job_t));;
    head->add(head, job1);
    job_t *job2 = malloc(sizeof(job_t));;
    head->add(head, job2);
    job_t *job3 = malloc(sizeof(job_t));;
    head->add(head, job3);
    for (int i = 0; i < 3; ++i) {
        head->add(head, job);
    }
    printf("Count: %d", head->count);
    return 0;
}
