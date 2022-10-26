#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

sem_t mutex;

int *stack, top;
int p, c, pc, cc;

void *produce(void *producer) {
    // sleep(5);
    int producer_no = *((int*)producer);
    int to_produce = pc;
    while(to_produce) {
        // Ensures the producer produces pc items
        sem_wait(&mutex); // Modifying the stack is the critical section
        int next_item = rand()%(1000);
        stack[top++] = next_item; // push onto stack
        to_produce--;
        printf("Producer %d produced %d\n", producer_no, next_item);
        sem_post(&mutex);
    }
}

void *consume(void *consumer) {
    int consumer_no = *((int*)consumer);
    int to_consume = cc;
    while(to_consume) {
        // Ensures the consumer consumes cc items
        sem_wait(&mutex);
        if(top) {
            // To not pop from empty stack
            int prev_item = stack[--top]; // pop from stack
            to_consume--;
            printf("Consumer %d consumed %d\n", consumer_no, prev_item);
        }
        sem_post(&mutex);
    }
}


int main(int argc, char *argv[]) {
    srand(time(NULL));

    if(argc != 9) {
        printf("Arguments not specified properly.\n");
        printf("Usage: ./multi_pc -p <p> -pc <pc> -c <c> -cc <cc>\n");
        exit(1);
    }

    for(int i = 1; i < argc; i++) {
        // Get the command line arguments
        if(!strcmp(argv[i], "-p")) p = atoi(argv[++i]);
        else if(!strcmp(argv[i], "-c")) c = atoi(argv[++i]);
        else if(!strcmp(argv[i], "-pc")) pc = atoi(argv[++i]);
        else if(!strcmp(argv[i], "-cc")) cc = atoi(argv[++i]);
    }

    if(p < 0 || c < 0 || pc < 0 || cc < 0) {
        printf("The arguments may not have negative values. Please enter positive values\n");
    }

    stack = (int*) malloc(p*pc*sizeof(int));
    top = 0;
    sem_init(&mutex, 0, 1);

    pthread_t producers[p], consumers[c];
    // Threads for the producers and consumers

    int producer_no[p], consumer_no[p];

    for(int i = 0; i < p; i++) producer_no[i] = i+1;
    for(int i = 0; i < c; i++) consumer_no[i] = i+1;

    for(int i = 0; i < p; i++) pthread_create(&producers[i], NULL, produce, &producer_no[i]);
    for(int i = 0; i < c; i++) pthread_create(&consumers[i], NULL, consume, &consumer_no[i]);
    // Create threads for producers and consumers and provide their corresponding functions

    for(int i = 0; i < p; i++) pthread_join(producers[i], NULL);
    for(int i = 0; i < c; i++) pthread_join(consumers[i], NULL);
    // Wait for all the threads to terminate

    free(stack);

    return 0;
}