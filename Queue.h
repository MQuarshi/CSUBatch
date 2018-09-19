//
// Created by jazart on 9/19/2018.
//
#include <time.h>
#include <stdlib.h>

#ifndef CSUBATCH_QUEUE_H
#define CSUBATCH_QUEUE_H

#endif //CSUBATCH_QUEUE_H


typedef struct Job {
    char *name;
    int priority;
    time_t sub_time;
    timer_t run_time;

} job_t;

typedef struct Queue_Node {
    int count;
    job_t *job;
    struct Queue_Node *next;

    struct Queue_Node (*remove_head)(struct Queue_Node *self);

    void (*add)(struct Queue_Node *, job_t *job);
} queue_t;

queue_t *remove_head(queue_t *self) {
    queue_t *head = self;
    self = self->next;
    self->count--;
    return head;
}

void add(queue_t *head, job_t *job) {
    queue_t *curr = head;
    while (curr->next != NULL) curr = curr->next;
    curr->next = malloc(sizeof(queue_t));
    curr->next->job = job;
    curr->next->next = NULL;
    head->count++;
}