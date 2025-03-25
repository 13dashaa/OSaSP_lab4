#include "setup_teardown.h"
#include "producer.h"
#include "consumer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

static const char options[] = {"Options:\n"
                               "[P] - create producer\n"
                               "[p] - delete producer\n"
                               "[C] - create consumer\n"
                               "[c] - delete consumer\n"
                               "[s] - show status\n"
                               "[q] - quit"};

msg_queue *queue = NULL;
int semid = -1;
pid_t producers[MAX_CHILD] = {0};
pid_t consumers[MAX_CHILD] = {0};
size_t producers_amount = 0;
size_t consumers_amount = 0;



void print_status() {
    printf("\n--- Current Status ---\n");
    printf("Producers: %zu\n", producers_amount);
    printf("Consumers: %zu\n", consumers_amount);
    printf("Messages in queue: %zu/%d\n", queue->msg_amount, MAX_MASSAGES);
    printf("Total added: %zu\n", queue->add_amount);
    printf("Total extracted: %zu\n", queue->extract_amount);
    printf("----------------------\n");
}

void set_nonblock_mode(int enable) {
    static struct termios oldt, newt;
    
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

int main(void) {

    init();
    
    set_nonblock_mode(1);
    
    printf("%s\n", options);
    
    while (1) {
        int c = getchar();
        
        if (c != EOF) {
            switch (c) {
                case 'P':
                    if (producers_amount < MAX_CHILD) {
                        create_producer();
                    } else {
                        printf("Max producers reached (%d)\n", MAX_CHILD);
                    }
                    break;
                    
                case 'p':
                    if (producers_amount > 0) {
                        remove_producer();
                    } else {
                        printf("No producers to remove\n");
                    }
                    break;
                    
                case 'C':
                    if (consumers_amount < MAX_CHILD) {
                        create_consumer();
                    } else {
                        printf("Max consumers reached (%d)\n", MAX_CHILD);
                    }
                    break;
                    
                case 'c':
                    if (consumers_amount > 0) {
                        remove_consumer();
                    } else {
                        printf("No consumers to remove\n");
                    }
                    break;
                    
                case 's':
                    print_status();
                    break;
                    
                case 'q':
                    cleanup();
                    set_nonblock_mode(0);
                    exit(EXIT_SUCCESS);
                    
                case '\n':
                    break;
                    
                default:
                    printf("Invalid option. %s\n", options);
                    break;
            }
        }
        
        usleep(700000);
    }
}