#include "consumer.h"

extern msg_queue *queue;
extern int semid;  
extern pid_t consumers[];
extern size_t consumers_amount;


void create_consumer(void) {
    if (consumers_amount == MAX_CHILD - 1) {
        fputs("Max value of consumers\n", stderr);
        return;
    }

    switch (consumers[consumers_amount] = fork()) {
        default:
            ++consumers_amount;
            return;

        case 0:
            break;

        case -1:
            perror("fork");
            exit(errno);
    }

    message msg;
    size_t extract_count_local;
    while (1) {
        sem_lock(QUEUED_ITEMS_SEM);

        sem_lock(MUTEX_SEM);
        extract_count_local = get_message(&msg);
        sem_unlock(MUTEX_SEM);

        sem_unlock(FREE_SLOTS_SEM);
        consume_message(&msg);

        printf("pid: %d consume msg: hash=%X, extract_count=%zu\n",
               getpid(), msg.hash, extract_count_local);

        sleep(5);
    }
}

void remove_consumer(void) {
    if (consumers_amount == 0) {
        fputs("No consumers to delete\n", stderr);
        return;
    }

    --consumers_amount;
    kill(consumers[consumers_amount], SIGKILL);
    wait(NULL);
}

void consume_message(message *msg) {
    uint16_t msg_hash = msg->hash;
    msg->hash = 0;
    uint16_t check_sum = calculate_hash(msg);
    if (msg_hash != check_sum) {
        fprintf(stderr, "check_sum=%X not equal msg_hash=%X\n",
                check_sum, msg_hash);
    }
    msg->hash = msg_hash;
}