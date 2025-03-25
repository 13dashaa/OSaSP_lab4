#ifndef PRODUCER_H_
#define PRODUCER_H_

#include "prodcons_queue.h"

#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>

void create_producer(void);

void remove_producer(void);

void produce_message(message *msg);

#endif 