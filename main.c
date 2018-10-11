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

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;
pthread_cond_t emptyQ = PTHREAD_COND_INITIALIZER;
pthread_t tid;

/* Error Code */
#define EINVAL       1
#define E2BIG        2

#define MAXMENUARGS  4
#define MAXCMDLINE   64

struct Queue_Node* job_queue;
void menu_execute(char *line, int isargs);
int cmd_run(int nargs, char **args);

int cmd_list(int nargs, char **args);
int cmd_quit(int nargs, char **args);
void showmenu(const char *name, const char *x[]);
int cmd_helpmenu(int n, char **a);
int cmd_dispatch(char *cmd);
char sched[8] = "fcfs";

/*
 * The run command - submit a job.
 */
int cmd_run(int nargs, char **args) {
    job_t* job = malloc(sizeof(job_t));
    if (nargs != 4) {
        printf("Usage: run <job> <time> <priority>\n");
        return EINVAL;
    }

//    strcpy(job_queue->job->name, args[1]);
    int burst = atoi(args[2]);
    int priority = atoi(args[3]);
    job->run_time = burst;
    job->priority = priority;
    job_queue->add(job_queue, job);

    if (job_queue->count == 1) {
        pthread_cond_signal(queue_mutex);
    }
    /* Use execv to run the submitted job in csubatch */
    pid_t child = fork();
    char *execv_args[] = {"C:\\Users\\jazart\\CLionProjects\\CSUBatch\\job.exe", NULL };
    if(child == 0) {
        if(execv("C:\\Users\\jazart\\CLionProjects\\CSUBatch\\job.exe",
                execv_args) < 0) {
            printf("\nError\n");
            exit(0);
        }
    }

    if(child != 0) {
        waitpid(-1, NULL, 0);
    }
    printf("\nuse execv to run the job in csubatch.\n");
    return 1; /* if succeed */
}

/*
 * The quit command.
 */
int cmd_quit(int nargs, char **args) {
    printf("Please display performance information before exiting csubatch!\n");
    exit(0);
}

int cmd_sched(int nargs, char **args) {
    printf("Please display performance information before exiting csubatch!\n");
    exit(0);
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
        "[quit] Exit csubatch                 ",
        "[help] Print help menu              ",
        /* Please add more menu options below */
        "[list] List all jobs               ",
        NULL
};

int cmd_helpmenu(int n, char **a) {
    (void) n;
    (void) a;

    showmenu("csubatch help menu", helpmenu);
    return 0;
}

/*
 *  Command table.
 */
static struct {
    const char *name;
    int (*func)(int nargs, char **args);
} cmdtable[] = {
        /* commands: single command must end with \n */
        {"?\n",    cmd_helpmenu},
        {"h\n",    cmd_helpmenu},
        {"help\n", cmd_helpmenu},
        {"r",      cmd_run},
        {"run",    cmd_run},
        {"q\n",    cmd_quit},
        {"quit\n", cmd_quit},
        {"list\n", cmd_list},
        {"FCFS\n", cmd_sched},
        {"SJF\n", cmd_sched},
        {"Priority\n", cmd_sched},
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
    printf("Order\tJob Name\tBurst Time\tPriority\n");
    queue_t* jobs = job_queue->next;
    int count = job_queue->count;
    for (i = 0; i < count; i++) {
        printf("%" PRId64 "\t%s\t\t%ld\t\t%d\n", jobs->job->sub_time, jobs->job->name, jobs->job->run_time, jobs->job->priority);
        jobs = jobs->next;
    }
    return (0);
}
/*
 * Process a single command.
 */
int cmd_dispatch(char *cmd) {
    time_t beforesecs, aftersecs, secs;
    u_int32_t beforensecs, afternsecs, nsecs;
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

/*
 * Command line main loop.
 */
int main() {
    char *buffer;
    size_t bufsize = 64;
    job_queue = init_queue(job_queue);


    buffer = (char *) malloc(bufsize * sizeof(char));
    if (buffer == NULL) {
        perror("Unable to malloc buffer");
        exit(1);
    }

    while (1) {
        printf("> [? for menu]: ");
        getline(&buffer, &bufsize, stdin);
        cmd_dispatch(buffer);
    }
    return 0;
}

void create_modules(void *module) {
    pthread_create(&tid, NULL, module, NULL);
}

void schedulerMod(char *string) {

    //pthread_mutex_lock(queue_mutex);
    while (job_queue->count == 0) {
        pthread_cond_wait(emptyQ, queue_mutex);
    }
    if (strcmp(string, "FCFS") == 0) {
        sort(job_queue, 2);
    }

    if (strcmp(string, "Priority") == 0) {
        sort(job_queue, 1);
    }

    if (strcmp(string, "SJF") == 0) {
        sort(job_queue, 3);
    }


}

void dispatcherMod() {
    while (job_queue->count != 0) {
        execv(remove_head(job_queue)->job->name, 1);
    }
}