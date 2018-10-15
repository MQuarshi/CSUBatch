/*
 *
 * Compilation Instruction:
 * gcc commandline_parser.c -o commandline_parser
 * ./commandline_parser
 *
 */

#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <wait.h>
#include <inttypes.h>
#include "Queue.h"
#include <pthread.h>

pthread_mutex_t lock_queue; //Mutex to handle access to the queue
pthread_cond_t queue_not_empty; //Mutex used to make scheduler thread wait if queue is empty
pthread_cond_t change_policy;
pthread_t command_thread, dispatch_thread;

/* Error Code */
#define EINVAL       1
#define E2BIG        2

#define MAXMENUARGS  6
#define MAXCMDLINE   64

struct Queue_Node *job_queue;

int cmd_run(int nargs, char **args);

int cmd_list(int nargs, char **args);

int cmd_quit(int nargs, char **args);

void create_modules();

void *schedulerMod();

void *dispatcherMod();

void showmenu(const char *name, const char *x[]);

int cmd_helpmenu(int n, char **a);

int cmd_dispatch(char *cmd);

char sched[15] = "FCFS";
int resched = 0;
/*
 * The run command - submit a job.
 */
int cmd_run(int nargs, char **args) {
    job_t *job = malloc(sizeof(job_t));
    if (nargs != 4) {
        printf("Usage: run <job> <time> <priority>\n");
        return EINVAL;
    }

    int burst = atoi(args[2]);
    int priority = atoi(args[3]);
    job->run_time = burst;
    job->priority = priority;
    strcpy(job->name, args[1]);

    if (job_queue->count < 1) {
        job->sub_time = currentTimeMillis();
        job_queue->job = job;
        job_queue->count++;
        pthread_cond_signal(&queue_not_empty);
    } else {
        job_queue->add(job_queue, job);
    }

    if (job_queue->count == 1) {
    }

    printf("\nuse execv to run the job in csubatch.\n");
    return 1;
}


int cmd_sched(int nargs, char **args) {
    strcpy(sched, args[0]);
    resched = 1;
    pthread_cond_signal(&change_policy);
    return 1;
}

/*
 * Display menu information
 */
void showmenu(const char *name, const char *x[]) {
    int ct, half, i;

    printf("\n");
    printf("%s\n", name);

    for (i = ct = 0; x[i]; i++) {
        ct++;
    }
    half = (ct + 1) / 2;

    for (i = 0; i < half; i++) {
        printf("    %-36s", x[i]);
        if (i + half < ct) {
            printf("%s", x[i + half]);
        }
        printf("\n");
    }

    printf("\n");
}


static const char *helpmenu[] = {
        "[run] <job> <time> <priority>       ",
        "[test] <benchmark> <policy> <num_of_jobs> <priority_levels>\n<min_CPU_time> <max_CPU_time>",
        "[quit] Exit csubatch                 ",
        "[help] Print help menu              ",
        "[list] List all jobs               ",
        NULL
};

int cmd_helpmenu(int n, char **a) {
    (void) n;
    (void) a;

    showmenu("csubatch help menu", helpmenu);
    return 0;
}

int cmd_test(int nargs, char **args) {
    if (nargs != 6) {
        printf("Usage:[test] <benchmark> <policy> <num_of_jobs> <priority_levels>\n<min_CPU_time> <max_CPU_time>\",\n");
        return EINVAL;
    }
    return 1;
}

/*
 *  Command table.
 */
static struct {
    const char *name;

    int (*func)(int nargs, char **args);
} cmdtable[] = {
        /* commands: single command must end with \n */
        {"?\n",        cmd_helpmenu},
        {"h\n",        cmd_helpmenu},
        {"help\n",     cmd_helpmenu},
        {"r",          cmd_run},
        {"run",        cmd_run},
        {"q\n",        cmd_quit},
        {"quit\n",     cmd_quit},
        {"list\n",     cmd_list},
        {"FCFS\n",     cmd_sched},
        {"SJF\n",      cmd_sched},
        {"Priority\n", cmd_sched},
        {"test\n",     cmd_test},
        /* Please add more operations below. */
        {NULL, NULL}
};

/*
 * List out the jobs
 */
int cmd_list(int nargs, char **args) {
    int i;
    printf("The Current Scheduling policy is:%s\n", sched);
    printf("The following are the list of jobs:\n");
    printf("Sub Time  \tJob Name\tBurst Time\tPriority\n");
    queue_t *jobs = job_queue;
    while(jobs != NULL) {
        printf("%" PRId64 "\t%s\t\t%d\t\t%d\n", jobs->job->sub_time, jobs->job->name, jobs->job->run_time,
               jobs->job->priority);
        jobs = jobs->next;
    }
    return (0);
}

/*
 * after 'quit', display performance
 */
void dis_perfo() {
    int result_count = job_queue->count;
    queue_t *jobs = job_queue->next;
    double turnaround = 0;
    double burstTime = 0;
    if (result_count == 0) {
        printf("Program end\n");
        return;
    }
    for (int i = 0; i < result_count; i++) {
        turnaround = currentTimeMillis() - jobs->job->sub_time;
        burstTime += jobs->job->run_time;

    }
    printf("Total number jobs submitted: %d\n", result_count);
    printf("Average BurstTime:\t\t %.2f\n", burstTime / result_count);
    printf("Average waiting time:\t\t %.2f\n", (turnaround - burstTime) / result_count);
    printf("Average Throughput:\t\t %.3f\n", (1 / turnaround) / result_count);
}

/*
 * The quit command.
 */
int cmd_quit(int nargs, char **args) {
    dis_perfo();
    printf("Please display performance information before exiting csubatch!\n");
    exit(0);
}

/*
 * Process a single command.
 */
int cmd_dispatch(char *cmd) {
    char *args[MAXMENUARGS];
    int nargs = 0;
    char *word;
    char *context;
    int i, result;

    for (word = strtok_r(cmd, " ", &context);
         word != NULL;
         word = strtok_r(NULL, " ", &context)) {

        if (nargs >= MAXMENUARGS) {
            printf("Command line has too many words\n");
            return E2BIG;
        }
        args[nargs++] = word;
    }

    if (nargs == 0) {
        return 0;
    }

    for (i = 0; cmdtable[i].name; i++) {
        if (*cmdtable[i].name && !strcmp(args[0], cmdtable[i].name)) {
            assert(cmdtable[i].func != NULL);

            /* Call function through the cmd_table */
            result = cmdtable[i].func(nargs, args);
            return result;
        }
    }

    printf("%s: Command not found\n", args[0]);
    return EINVAL;
}

void create_modules() {
    pthread_create(&command_thread, NULL, schedulerMod, NULL);
    pthread_create(&dispatch_thread, NULL, dispatcherMod, NULL);


}

//Scheduler thread that works, but doesn't run concurrently
void *dispatcherMod() {

    //While it loops, if the quit commans is used, then it will exit
    while (job_queue->count == 0) {
        pthread_cond_wait(&queue_not_empty, &lock_queue);
    }

    FILE *f = fopen("C:\\Users\\jazart\\CLionProjects\\CSUBatch\\hello.txt", "w+");
    pid_t child = fork();
    char *execv_args[] = {"C:\\Users\\jazart\\CLionProjects\\CSUBatch\\hello.exe", NULL};
    if (child == 0) {
        dup2(fileno(f), STDOUT_FILENO);
        fclose(f);
        if (execv("C:\\Users\\jazart\\CLionProjects\\CSUBatch\\hello.exe",
                  execv_args) < 0) {
            printf("\nError\n");
            exit(0);
        }
        job_queue->remove_head(job_queue);
    }

    if (child != 0) {
        waitpid(-1, NULL, 0);
    }

    sleep(5);
    pthread_mutex_unlock(&lock_queue);

}

void *schedulerMod() {
    //if job queue is empty, the thread will wait until
    while (1) {
        while (resched != 1) {
            pthread_cond_wait(&change_policy, &lock_queue);
        }

        if (strcmp(sched, "FCFS\n") == 0) {
            job_queue = sort(job_queue, 2);
            printf("Sorted\n");
        }

        if (strcmp(sched, "Priority\n") == 0) {
            job_queue = sort(job_queue, 1);
            printf("Sorted\n");
        }

        if (strcmp(sched, "SJF\n") == 0) {
            printf("%s", sched);
            job_queue = sort(job_queue, 3);
            printf("Sorted\n");
        }
        queue_t *cpy = job_queue;
        resched = 0;
        pthread_mutex_unlock(&lock_queue);
    }
}

int main() {
    char *buffer;
    size_t bufsize = 64;
    job_queue = init_queue(job_queue);
    int bool = 1;

    buffer = (char *) malloc(bufsize * sizeof(char));
    if (buffer == NULL) {
        perror("Unable to malloc buffer");
        exit(1);
    }

    create_modules();
    while (bool == 1) {
        printf("> [? for menu]: ");
        getline(&buffer, &bufsize, stdin);
        if (strcmp(buffer, "quit") == 0) {
            bool = 0;
        }
        cmd_dispatch(buffer);
    }
    return 0;
}