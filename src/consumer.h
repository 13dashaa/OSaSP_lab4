#ifndef CONSUMER_H_
#define CONSUMER_H_

#include "prodcons_queue.h"

#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>

void create_consumer(void);

void remove_consumer(void);

void consume_message(message *msg);

#endif