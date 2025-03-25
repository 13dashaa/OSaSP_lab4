#ifndef PRODCONS_QUEUE_H_
#define PRODCONS_QUEUE_H_

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>

#define MAX_DATA (((256 + 3) / 4) * 4)
#define MAX_CHILD 1024
#define MAX_MASSAGES 4096

#define MUTEX_SEM      0
#define FREE_SLOTS_SEM 1
#define QUEUED_ITEMS_SEM 2

#define SHM_OBJ "/queue"
#define MUTEX "mutex"
#define FREE_SLOTS "free_slots"
#define QUEUED_ITEMS "queued_items"


typedef struct
{
    int8_t type;
    uint16_t hash;
    uint8_t size;
    char data[MAX_DATA];
} message;

typedef struct{

    size_t add_amount;
    size_t extract_amount;
    
    size_t msg_amount;
    
    size_t head;
    size_t tail;
    message buffer[MAX_MASSAGES];

}msg_queue;

uint16_t calculate_hash(message *msg);

void msg_queue_init(void);

size_t put_message(message *msg);

size_t get_message(message *msg);

void sem_lock(int sem_num);
void sem_unlock(int sem_num);
#endif