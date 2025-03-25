#include "producer.h"

extern msg_queue *queue;
extern int semid;  
extern pid_t producers[];
extern size_t producers_amount;


void create_producer(void) {
    if (producers_amount == MAX_CHILD - 1) {
        fputs("Max value of producers\n", stderr);
        return;
    }

    switch (producers[producers_amount] = fork()) {
        default:
            ++producers_amount;
            return;

        case 0:
            srand(getpid());
            break;

        case -1:
            perror("fork");
            exit(errno);
    }

    message msg;
    size_t add_count_local;
    while (1) {
        produce_message(&msg);


        sem_lock(FREE_SLOTS_SEM);

        sem_lock(MUTEX_SEM);
        add_count_local = put_message(&msg);
        sem_unlock(MUTEX_SEM);

        sem_unlock(QUEUED_ITEMS_SEM);

        printf("pid: %d produce msg: hash=%X, add_count=%zu\n",
               getpid(), msg.hash, add_count_local);

        sleep(5);
    }
}

void remove_producer(void) {
    if (producers_amount == 0) {
        fputs("No producers to delete\n", stderr);
        return;
    }

    --producers_amount;
    kill(producers[producers_amount], SIGKILL);
    wait(NULL);
}

void produce_message(message *msg) {
    size_t value = rand() % 257;
    if (value == 256) {
        msg->type = -1;
    } else {
        msg->type = 0;
        msg->size = (uint8_t) value;
    }

    for (size_t i = 0; i < value; ++i) {
        msg->data[i] = (char) (rand() % 256);
    }
    msg->hash = 0;
    msg->hash = calculate_hash(msg);
}