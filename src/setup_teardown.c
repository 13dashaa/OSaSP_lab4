#include "setup_teardown.h"

#include <sys/ipc.h>
#include <sys/shm.h>

extern int semid; 
extern pid_t producers[];
extern size_t producers_amount;
extern pid_t consumers[];
extern size_t consumers_amount;
extern msg_queue *queue;
 
static pid_t parent_pid;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void init(void) {
    parent_pid = getpid();

    int shmid = shmget(SHM_KEY, sizeof(msg_queue), SHM_FLAGS);
    if (shmid == -1) {
        perror("shmget");
        exit(errno);
    }


    queue = (msg_queue *)shmat(shmid, NULL, 0);
    if (queue == (void *)-1) {
        perror("shmat");
        exit(errno);
    }

    msg_queue_init();

    semid = semget(SEM_KEY, 3, SEM_FLAGS);
    if (semid == -1) {
        perror("semget");
        exit(errno);
    }

    union semun arg;
    arg.val = 1;
    if (semctl(semid, MUTEX_SEM, SETVAL, arg) == -1) {  
        perror("semctl mutex");
        exit(errno);
    }

    arg.val = MAX_MASSAGES; 
    if (semctl(semid, FREE_SLOTS_SEM, SETVAL, arg) == -1) {
        perror("semctl free_space");
        exit(errno);
    }

    arg.val = 0;     
    if (semctl(semid, QUEUED_ITEMS_SEM, SETVAL, arg) == -1) {
        perror("semctl items");
        exit(errno);
    }
}

void cleanup(void) {
    if (getpid() == parent_pid) {
        for (size_t i = 0; i < producers_amount; ++i) {
            kill(producers[i], SIGKILL);
            wait(NULL);
        }
        for (size_t i = 0; i < consumers_amount; ++i) {
            kill(consumers[i], SIGKILL);
            wait(NULL);
        }

        int shmid = shmget(SHM_KEY, 0, 0);
        if (shmid != -1) {
            shmctl(shmid, IPC_RMID, NULL);
        }

        if (semid != -1) {
            semctl(semid, 0, IPC_RMID);
        }
    }
}