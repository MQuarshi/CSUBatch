#include <stdio.h>
#include "Queue.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
/**
 * Sorts based on type, 1 = priority, 2 = sjf, 3 = fcfs
 */
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;
pthread_t tid;

int main() {
    queue_t *head = init_queue(head);
    job_t *job = malloc(sizeof(job_t));;
    head->job = job;

    for(int i = 0; i < 5; ++i) {
        job_t *j = malloc(sizeof(job_t));;
        j->priority = i + 1;
        j->run_time = i * i;
        head->add(head, j);
    }
    head = sort(head, 1);
    for(int i = 0; i < 5; ++i) {
        printf("\nPriority: %d, Sub Time: %ld, Run Time: %ld\n", head->job->priority, head->job->sub_time, head->job->run_time);
        head = head->next;
    }
    printf("Count: %d", head->count);
    return 0;
}

void create_modules(void *module1, void *module2) {
    pthread_create(&tid, NULL, module1, (void *) &tid);
    pthread_create(&tid, NULL, module2, (void *) &tid);
}

