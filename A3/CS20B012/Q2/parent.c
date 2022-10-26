#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<time.h>
#include<string.h>

#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500
#define KILL 600

int main()
{
    const long Request = REQUEST;
    const long Pivot = PIVOT;
    const long Large = LARGE;
    const long Small = SMALL;
    const long Ready = READY;
    const long Kill = KILL;
    srand(time(0));

    // Named pipes
    char *readfifo[5] = {"ChildW1", "ChildW2", "ChildW3", "ChildW4", "ChildW5"};
    char *writefifo[5] = {"ChildR1", "ChildR2", "ChildR3", "ChildR4", "ChildR5"};
    char *path = strdup("./child");
    int fdw[5], fdr[5];

    // Forking and calling execlp 5 times
    for(int i = 0; i < 5; i++) {
        long fileno = i+1;
        mkfifo(writefifo[i], 0666);
        mkfifo(readfifo[i], 0666);
        int pid = fork();
        if(pid < 0)
            exit(1);
        if(pid == 0) {  // Exec called and names of pipes passed to child
            execlp(path, path, readfifo[i], writefifo[i], (char *) NULL);
        }
        fdw[i] = open(writefifo[i], O_WRONLY);
        write(fdw[i], &fileno, sizeof(long));   // Sending child number through pipe
        close(fdw[i]);
    }

    // Waiting for children to read files
    long code;
    for(int i = 0; i < 5; i++) {
        while(1) {
            fdr[i] = open(readfifo[i], O_RDONLY);
            read(fdr[i], &code, sizeof(long));
            close(fdr[i]);
            if(code == Ready) {
                printf("Parent receives READY from Child %d\n", i+1);
                break;
            }
            else printf("Received %ld. Waiting...\n", code);
        }
    }
    printf("Parent READY\n");

    long k = 12;
    long pivot;
    while(1) {
        sleep(1);   // Parent waits for child to complete the current operation as this becomes a problem sometimes
        long childno, m = 0;

        // Sending requests
        while(1) {
            childno = rand()%5;
            fdw[childno] = open(writefifo[childno], O_WRONLY);
            write(fdw[childno], &Request, sizeof(Request));
            close(fdw[childno]);
            printf("Parent sends REQUEST to Child %ld\n", childno+1);
            fdr[childno] = open(readfifo[childno], O_RDONLY);
            read(fdr[childno], &pivot, sizeof(pivot));
            close(fdr[childno]);
            if(pivot != -1) //Pivot found
                break;
        }
        printf("Parent broadcasts %ld to all children\n", pivot);

        long temp[5];
        for(int i = 0; i < 5; i++) {
            fdw[i] = open(writefifo[i], O_WRONLY);
            write(fdw[i], &Pivot, sizeof(Pivot));
            write(fdw[i], &pivot, sizeof(pivot));
            close(fdw[i]);
        }
        for(int i = 0; i < 5; i++) {
            fdr[i] = open(readfifo[i], O_RDONLY);
            read(fdr[i], &temp[i], sizeof(temp[i]));
            close(fdr[i]);
            m += temp[i];
        }
        printf("Parent: m=%ld+%ld+%ld+%ld+%ld=%ld. ", temp[0], temp[1], temp[2], temp[3], temp[4], m);
        
        if(m == k) {    //Median found => break
            printf("%ld=k=%ld. Median found: %ld!\n", m, k, pivot);
            break;
        }
        else if(m > k) {    //Sum is large, so we can delete smaller values
            printf("%ld>k=%ld. Parent issues SMALL signal to all children\n", m, k);
            for(int i = 0; i < 5; i++) {
                fdw[i] = open(writefifo[i], O_WRONLY);
                write(fdw[i], &Small, sizeof(Small));
                close(fdw[i]);
            }
        }
        else {  //Sum is small, so we can delete larger values
            printf("%ld<k=%ld. Parent issues LARGE signal to all children\n", m, k);
            for(int i = 0; i < 5; i++) {
                fdw[i] = open(writefifo[i], O_WRONLY);
                write(fdw[i], &Large, sizeof(Large));
                close(fdw[i]);
            }
            k -= m;
        }
    }

    printf("Parent sends kill signal to all children\n");
    for(int i = 0; i < 5; i++)
    {
        fdw[i] = open(writefifo[i], O_WRONLY);
        write(fdw[i], &Kill, sizeof(Kill));
        close(fdw[i]);
    }
    for(int i = 0; i < 5; i++)
        wait(NULL);
    return 0;
}