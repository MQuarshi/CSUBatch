#include <stdio.h>
#include "Queue.h"
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;
pthread_t tid;

int main() {

    char option = 0;
    while (option != '4') {

        printf("\n\tMENU DEMONSTRATION");
        printf("\n\t------------------------------");
        printf("\n\n\t 1. TEST PROGRAM");
        printf("\n\t 2. HELP");
        printf("\n\t 3. RUN PROGRAM");
        printf("\n\t 4. EXIT");
        printf("\n\n Enter Your Choice: ");
        option = scanf("%c", &option);
        switch (option) {
            case '1':
                printf("\n\nYOU SELECTED OPTION 1:TEST PROGRAM\n");
                break;
            case '2':
                printf("\n\nYOU SELECTED OPTION 2: HELP COMMAND\n");
                break;
            case '3':
                printf("\n\nYOU SELECTED OPTION 3:RUN PROGRAM\n");
                break;
            case '4':
                printf("\n\nYOU SELECTED OPTION 4:EXIT\n");
                break;
            default:
                printf("\n\nINVALID SELECTION...Please try again\n");
        }
    }


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

void create_modules(void *module1, void *module2) {

    pthread_create(&tid, NULL, module1, (void *) &tid);
    pthread_create(&tid, NULL, module2, (void *) &tid);

}

