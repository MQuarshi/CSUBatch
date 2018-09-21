#include <stdio.h>
#include "Queue.h"
#include <pthread.h>

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;
pthread_t tid;

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

void createModules(void *module1, void *module2) {

    pthread_create(&tid, NULL, module1, (void *) &tid);
    pthread_create(&tid, NULL, module2, (void *) &tid);

}

//void *schedulerMod () {

//int resVal;

//Implementation not done yet

//resVal = pthread_cond_signal(&condB);

//resVal = pthread_mutex_lock(&queueMutex);

//while (pthread_cond_wait(&condA, &queueMutex) != 0)

// }

//void *dispatcherMod () {

//Implementation not done yet

//while (pthread_cond_wait(&condB, &queueMutex) != 0)

// rValue = pthread_mutex_unlock(&mutex);

//}
