#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<unistd.h>

#define THINKING 0
#define EATING 1

int *chopsticks;
sem_t mutex, *philSem;

typedef struct Philosopher {
    int id;
    int state;
    int nextStateTime;
    int eatState;
    int left, right;
    int currTime;
} Philosopher;

Philosopher *philosophers;

void set_values(Philosopher *philosopher, int philosopherId, int N) {
    philosopher->id = philosopherId;
    philosopher->state = THINKING;
    philosopher->nextStateTime = 50 + rand()%51;
    philosopher->eatState = 0;
    philosopher->left = philosopherId;
    philosopher->right = (philosopherId + 1)%N;
    philosopher->currTime = 0; // maintains the current time for each philosopher
}

int checkThink(Philosopher *philosopher) {
    // Checks if ready to enter THINKING state
    return (philosopher->nextStateTime==philosopher->currTime && philosopher->state==EATING);
}

int checkEat(Philosopher *philosopher) {
    // Checks if ready to enter EATING state
    if(philosopher->nextStateTime != philosopher->currTime || philosopher->state == EATING || philosopher->eatState > 5) return 0;
    return (chopsticks[philosopher->left] == 1 && chopsticks[philosopher->right] == 1);
}

void printThinking(Philosopher *philosopher) {
    printf("<%d> Philosopher %d in THINKING state\n", philosopher->currTime, philosopher->id);
}

void printEating(Philosopher *philosopher) {
    printf("<%d> Philosopher %d in EATING state - %d\n", philosopher->currTime, philosopher->id, philosopher->eatState);
}

void startThinking(Philosopher *philosopher) {
    sem_wait(&mutex);
    if(checkThink(philosopher)) {
        // Entered when the philosopher has finished eating and is ready to think
        chopsticks[philosopher->left] = 1;
        chopsticks[philosopher->right] = 1;
        sem_post(&philSem[philosopher->id]);
        philosopher->state = THINKING;
        philosopher->eatState++;
        printThinking(philosopher);
        philosopher->nextStateTime = philosopher->currTime + 50 + rand()%51;
    }
    sem_post(&mutex);
    usleep(1000 * (philosopher->nextStateTime - philosopher->currTime)); // in order to allow the threads to synchronize their currTimes
    // Always runs whenever nextStateTime is initialized or updated
    philosopher->currTime = philosopher->nextStateTime;
}

void startEating(Philosopher *philosopher) {
    sem_wait(&mutex); // Checking and modifying chopsticks availability is the critical section
    if(checkEat(philosopher)) {
        // Entered when the philosopher has finished thinking and is ready to eat
        chopsticks[philosopher->left] = 0;
        chopsticks[philosopher->right] = 0;
        sem_wait(&philSem[philosopher->id]);
        philosopher->state = EATING;
        printEating(philosopher);
        philosopher->nextStateTime = philosopher->currTime + 100;
    }
    else if(philosopher->nextStateTime == philosopher->currTime && philosopher->state == THINKING){
        // Entered when the philosopher wants to eat but can't due to unavailability of chopsticks
        philosopher->nextStateTime = philosopher->currTime + 50 + rand()%51;
    }
    sem_post(&mutex);
}

void *diningPhilosopher(void *philosopher) {
    Philosopher *philosopher_ = (Philosopher*) philosopher;
    printThinking(philosopher_);
    while(philosopher_->eatState != 6) {
        // As long as the philosopher hasn't finished eating 5 times, keep thinking and eating
        startThinking(philosopher_);
        startEating(philosopher_);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if(argc != 2) {
        printf("Expected one command line argument: N, received %d arguments\n", argc-1);
        exit(1);
    }
    int N = atoi(argv[1]); // Get the value of N
    if(N < 2) {
        printf("Error: There cannot be %d philosophers. Please provide an argument of at least 2\n", N);
        exit(1);
    }

    philosophers = (Philosopher*) malloc(N * sizeof(Philosopher));
    pthread_t threadId[N]; // A thread for each philosopher
    philSem = (sem_t*) malloc(N * sizeof(sem_t)); // A semaphore for each philosopher
    chopsticks = (int *) malloc(N * sizeof(int));

    for(int i = 0; i < N; i++)
        set_values(&philosophers[i], i, N);

    for(int i = 0; i < N; i++)
        chopsticks[i] = 1;

    sem_init(&mutex, 0, 1);

    for(int i = 0; i < N; i++) sem_init(&philSem[i], 0, 1);

    // All semaphores are binary, initialized with 1 to say that the critical section is not locked initally

    for(int i = 0; i < N; i++)
        // Threads created and assigned their function
        pthread_create(&threadId[i], NULL, diningPhilosopher, &philosophers[i]);


    for(int i = 0; i < N; i++)
        // Wait for the threads to terminate
        pthread_join(threadId[i], NULL);

    free(philosophers);
    free(philSem);
    free(chopsticks);

    return 0;
}