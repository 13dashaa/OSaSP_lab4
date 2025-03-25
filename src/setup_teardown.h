#ifndef SETUP_TEARDOWN_H_
#define SETUP_TEARDOWN_H_

#include "prodcons_queue.h"

#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>

#define SHM_KEY 0x1234  
#define SEM_KEY 0x5678  
#define SHM_FLAGS (IPC_CREAT | 0666)  
#define SEM_FLAGS (IPC_CREAT | 0666) 

void init(void);
void cleanup(void);

#endif