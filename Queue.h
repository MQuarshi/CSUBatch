//
// Created by jazart on 9/19/2018.
//
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#ifndef CSUBATCH_QUEUE_H
#define CSUBATCH_QUEUE_H

#endif //CSUBATCH_QUEUE_H


typedef struct Job {
    char name [20];
    char* const namargs;
    int priority;

    int64_t sub_time; //arrival
    int run_time;//burst time

} job_t;

typedef struct Queue_Node {
    unsigned int count;
    job_t *job;
    struct Queue_Node *next;
    struct Queue_Node (*remove_head)(struct Queue_Node *self);
    void (*add)(struct Queue_Node *, job_t *job);
} queue_t;

queue_t *init_queue(queue_t *head);

int64_t currentTimeMillis();

queue_t *remove_head(queue_t *self) {
    queue_t *head = self;
    if (self->next != NULL) {
        self = self->next;
        self->count--;
    } else {
        self = init_queue(self);
    }
    return head;
}

void (add)(queue_t *head, job_t *job) {
    queue_t *curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }

    struct timeval start, stop;
    double secs = 0;

    job->sub_time = currentTimeMillis();
    curr->next = malloc(sizeof(queue_t));
    curr->next->job = job;
    curr->next->next = NULL;
    head->count++;
    curr->count = head->count;
}

queue_t* init_queue(queue_t* head) {
    head = malloc(sizeof(queue_t));
    head->count = 0;
    head->add = add;
    head->next = NULL;
    return head;
}

queue_t* sort(queue_t* jobs, int type) {
    if(jobs == NULL || jobs->next == NULL) return jobs;

    queue_t* q = jobs;
    queue_t* p = jobs->next->next;
    while(p && p->next) {
        q = q->next;
        p = p->next->next;
    }

    queue_t* mid = q->next;
    q->next = NULL;
    queue_t* fast = sort(jobs, type);
    queue_t* slow = sort(mid, type);
    queue_t ret[1];
    queue_t* tail = ret;
    switch(type) {
        /// sort by priority
        case 1:
            while(fast != NULL && slow != NULL) {
                if(fast->job->priority > slow->job->priority) {
                    tail->next = fast;
                    tail = fast;
                    fast = fast->next;
                } else {
                    tail->next = slow;
                    tail = slow;
                    slow = slow->next;
                }
            }
            break;
//            /// sort by submission time-fcfs
        case 2:
            while(fast != NULL && slow != NULL) {
                if (fast->job->sub_time < slow->job->sub_time) {
                    tail->next = fast;
                    tail = fast;
                    fast = fast->next;
                } else {
                    tail->next = slow;
                    tail = slow;
                    slow = slow->next;
                }
            }
            break;
            /// sort by job time-sjf
        case 3:
            while(fast != NULL && slow != NULL) {
                if (fast->job->run_time < slow->job->run_time) {
                    tail->next = fast;
                    tail = fast;
                    fast = fast->next;
                } else {
                    tail->next = slow;
                    tail = slow;
                    slow = slow->next;
                }
            }
            break;
        default:
            return jobs;
    }

    while(fast) {
        tail->next = fast;
        tail = fast;
        fast = fast->next;
    }

    while(slow) {
        tail->next = slow;
        tail = slow;
        slow = slow->next;
    }
    tail->next = NULL;
    return ret->next;
}


int64_t currentTimeMillis() {
    struct timeval time;
    gettimeofday(&time, NULL);
    int64_t s1 = (int64_t)(time.tv_sec) * 1000;
    int64_t s2 = (time.tv_usec / 1000);
    return s1 + s2;
}
