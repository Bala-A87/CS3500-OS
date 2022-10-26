#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int main() {
    FILE *fptr;
    srand(time(0));
    char *files[5] = {"data1.txt", "data2.txt", "data3.txt", "data4.txt", "data5.txt"};
    long vals[51];
    for(int i = 0; i < 51; i++)
        vals[i] = i;
    int end = 51;
    // for(int i = 0; i < 25; i++) { //Check!
    //     int curr = rand()%end;
    //     vals[i] = vals[i] ^ vals[curr];
    //     vals[curr] = vals[i] ^ vals[curr];
    //     vals[i] = vals[i] ^ vals[curr];
    //     end--;
    // }
    while(end > 0) {
        int curr = rand()%end;
        // vals[end] = vals[end] ^ vals[curr];
        // vals[curr] = vals[end] ^ vals[curr];
        // vals[end] = vals[end] ^ vals[curr];
        long temp = vals[end-1];
        vals[end-1] = vals[curr];
        vals[curr] = temp;
        end--;
    }
    int curr = 0;
    for(int i = 0; i < 5; i++) {
        fptr = fopen(files[i], "w");
        for(int j = 0; j < 5; j++) {
            fprintf(fptr, "%ld ", vals[curr]);
            curr++;
        }
        fclose(fptr);
    }
    long medianarray[25];
    for(int i = 0; i < 25; i++)
        medianarray[i] = vals[i];
    qsort(medianarray, 25, sizeof(long), compare);
    fptr = fopen("mediandata.txt", "w");
    for(int i = 0; i < 25; i++)
        fprintf(fptr, "%ld ", medianarray[i]);
    fprintf(fptr, "\n%ld\n", medianarray[12]);
    fclose(fptr);
    return 0;
}