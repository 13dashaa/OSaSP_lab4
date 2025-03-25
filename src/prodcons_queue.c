#include "prodcons_queue.h"

extern msg_queue *queue;
extern int semid;

uint16_t calculate_hash(message *msg) {

    uint32_t hash = 5381;
    const uint8_t *bytes = (const uint8_t*)msg;
    size_t total_size = sizeof(msg->type) + sizeof(msg->size) + msg->size;
    size_t hash_field_offset = offsetof(message, hash);
    for (size_t i = 0; i < total_size; ++i) {
        if (i >= hash_field_offset && i < hash_field_offset + sizeof(msg->hash)) {
            continue;
        }
        hash = ((hash << 5) + hash) + bytes[i];
    }
    return (uint16_t)(hash & 0xFFFF);
}

void msg_queue_init(void) {
    queue->add_amount = 0;
    queue->extract_amount = 0;
    queue->msg_amount = 0;
    queue->head = 0;
    queue->tail = 0;
    memset(queue->buffer, 0, sizeof(queue->buffer));
}

size_t put_message(message *msg) {
    if (queue->msg_amount == MAX_MASSAGES - 1) {
        fputs("Queue is full\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (queue->head == MAX_MASSAGES) {
        queue->head = 0;
    }

    queue->buffer[queue->head] = *msg;
    queue->head+=1;
    queue->msg_amount+=1;

    return ++queue->add_amount;
}

size_t get_message(message *msg) {
    if (queue->msg_amount == 0) {
        fputs("Queue is empty\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (queue->tail == MAX_MASSAGES) {
        queue->tail = 0;
    }

    *msg = queue->buffer[queue->tail];
    queue->tail+=1;
    queue->msg_amount-=1;

    return ++queue->extract_amount;
}

void sem_lock(int sem_num) {
    struct sembuf op = {sem_num, -1, 0};
    if (semop(semid, &op, 1)) {
        perror("semop lock");
        exit(EXIT_FAILURE);
    }
}

void sem_unlock(int sem_num) {
    struct sembuf op = {sem_num, 1, 0};
    if (semop(semid, &op, 1)) {
        perror("semop unlock");
        exit(EXIT_FAILURE);
    }
}