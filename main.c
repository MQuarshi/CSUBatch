#include <stdio.h>
#include "Queue.h"
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;
pthread_t tid;


#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* Error Code */
#define EINVAL       1
#define E2BIG        2

#define MAXMENUARGS  4
#define MAXCMDLINE   64

void menu_execute(char *line, int isargs);

int cmd_run(int nargs, char **args);

int cmd_quit(int nargs, char **args);

void showmenu(const char *name, const char *x[]);

int cmd_helpmenu(int n, char **a);

int cmd_dispatch(char *cmd);

/*
 * The run command - submit a job.
 */
int cmd_run(int nargs, char **args) {
    if (nargs != 4) {
        printf("Usage: run <job> <time> <priority>\n");
        return EINVAL;
    }

    int i = nargs;
    printf("%d", i);

    char *jobName;
    int priority;
    time_t
    t
    while ((i--) > 1)
        printf("%s\n", args[i]);
    /* Use execv to run the submitted job in csubatch */
    printf("use execv to run the job in csubatch.\n");
    return 0; /* if succeed */
}

/*
 * The quit command.
 */
int cmd_quit(int nargs, char **args) {
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
        /* Please add more operations below. */
        {NULL, NULL}
};

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

///int main() {
///
///    char option = 0;
///    while (option != '4') {
///
///        printf("\n\tMENU DEMONSTRATION");
///        printf("\n\t------------------------------");
///        printf("\n\n\t 1. TEST PROGRAM");
///        printf("\n\t 2. HELP");
///        printf("\n\t 3. RUN PROGRAM");
///        printf("\n\t 4. EXIT");
///        printf("\n\n Enter Your Choice: ");
///        scanf(" %c", &option);
///        switch (option) {
///            case '1':
///                printf("\n\nYOU SELECTED OPTION 1:TEST PROGRAM\n");
///                break;
///            case '2':
///                printf("\n\nYOU SELECTED OPTION 2: HELP COMMAND\n");
///                break;
///            case '3':
///                printf("\n\nYOU SELECTED OPTION 3:RUN PROGRAM\n");
///                break;
///            case '4':
///                printf("\n\nYOU SELECTED OPTION 4:EXIT\n");
///                break;
///            default:
///                printf("\n\nINVALID SELECTION...Please try again\n");
///        }
///    }
///
///
///    queue_t *head = init_queue(head);
///    job_t *job = malloc(sizeof(job_t));;
///    head->add(head, job);
///    job_t *job1 = malloc(sizeof(job_t));;
///    head->add(head, job1);
///    job_t *job2 = malloc(sizeof(job_t));;
///    head->add(head, job2);
///    job_t *job3 = malloc(sizeof(job_t));;
///    head->add(head, job3);
///    for (int i = 0; i < 3; ++i) {
///        head->add(head, job);
///    }
///    printf("Count: %d", head->count);
///    return 0;
///}

void create_modules(void *module1, void *module2) {

    pthread_create(&tid, NULL, module1, (void *) &tid);
    pthread_create(&tid, NULL, module2, (void *) &tid);

}
