#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<time.h>
#include<string.h>

#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500
#define KILL 600

int main(int argc, char *argv[])
{
    srand(time(0));
    const long Request = REQUEST;
    const long Pivot = PIVOT;
    const long Large = LARGE;
    const long Small = SMALL;
    const long Ready = READY;
    const long Kill = KILL;

    long childno;
    // First argument is file name, second is write pipe, third is read pipe
    char *writefifo = argv[1];
    char *readfifo = argv[2];
    int fdw, fdr;

    fdr = open(readfifo, O_RDONLY);
    read(fdr, &childno, sizeof(childno));
    close(fdr);
    char fileno = (int)childno + '0';   // Read child number from pipe

    // Opening and getting data from the corresponding file
    long data[5];
    char *DATA = strdup("data");
    strncat(DATA, &fileno, 1);
    strncat(DATA, ".txt", 5);
    FILE *datafile = fopen(DATA, "r");
    for(int i = 0; i < 5; i++)
        fscanf(datafile, "%ld", &data[i]);
    fclose(datafile);

    fdw = open(writefifo, O_WRONLY);
    write(fdw, &Ready, sizeof(Ready));
    close(fdw);
    printf("Child %ld sends READY\n", childno);

    int right = 4;
    long pivot;

    while(1) {
        long code;  // Getting the operation to perform
        fdr = open(readfifo, O_RDONLY);
        read(fdr, &code, sizeof(code));
        if(code == Pivot)
            read(fdr, &pivot, sizeof(pivot));
        close(fdr);

        if(code == Request) {
            printf("Child %ld receives REQUEST\n", childno);
            long val = -1;  // In case empty
            if(right >= 0) {
                val = data[rand()%(right+1)];
            }
            fdw = open(writefifo, O_WRONLY);
            write(fdw, &val, sizeof(val));
            close(fdw);
            printf("Child %ld sends %ld to parent\n", childno, val);
        }

        else if(code == Pivot) {
            long count = 0;
            for(int i = 0; i <= right; i++)
                if(data[i] > pivot)
                    count++;
            fdw = open(writefifo, O_WRONLY);
            write(fdw, &count, sizeof(count));
            close(fdw);
            printf("Child %ld receives pivot %ld and replies %ld\n", childno, pivot, count);
        } 

        else if(code == Small) {
            // Keep values geq pivot in the front
            int curr = 0;
            for(int i = 0; i <= right; i++)
                if(data[i] >= pivot) {
                    data[curr] = data[i];
                    curr++;
                }
            right = curr-1;
        }

        else if(code == Large) {
            // Keep values leq pivot in the front
            int curr = 0;
            for(int i = 0; i <= right; i++)
                if(data[i] <= pivot) {
                    data[curr] = data[i];
                    curr++;
                }
            right = curr-1;
        }

        else if(code == Kill){
            printf("Child %ld terminates\n", childno);
            break;
        }
    }
    
    return 0;
}