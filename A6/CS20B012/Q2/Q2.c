#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/resource.h>

#define ONCE 1
#define EACH 2

void transpose(int **matrix, int n) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < i; j++) {
            matrix[i][j] = matrix[i][j] ^ matrix[j][i];
            matrix[j][i] = matrix[i][j] ^ matrix[j][i];
            matrix[i][j] = matrix[i][j] ^ matrix[j][i];
        }
}


int main() {
    srand(time(NULL));
    int n;
    printf("Enter n value: ");
    scanf("%d", &n);
    int choice;
    printf("Enter Choice\n");
    printf("Memory Allocated once - 1\n");
    printf("Memory Allocated each time - 2\n");
    scanf("%d", &choice);

    int **matrix[5];
    struct rusage process_usage;
    getrusage(RUSAGE_SELF, &process_usage);
    long init_mem_usage = process_usage.ru_maxrss;

    switch(choice) {
    case ONCE:
        for(int iteration = 1; iteration <= 10; iteration++) {
            for(int matrix_no = 0; matrix_no < 5; matrix_no++) {
                if(iteration == 1) {
                    matrix[matrix_no] = (int**) malloc(n * sizeof(int*));
                    for(int i = 0; i < n; i++)
                        matrix[matrix_no][i] = (int*) malloc(n * sizeof(int));
                }
                for(int i = 0; i < n; i++)
                    for(int j = 0; j < n; j++)
                        matrix[matrix_no][i][j] = rand();
                transpose(matrix[matrix_no], n);
            }
            getrusage(RUSAGE_SELF, &process_usage);
            long page_faults = process_usage.ru_minflt;
            long mem_usage = process_usage.ru_maxrss;
            printf("memory usage: %ld + %ld, page_faults: %ld\n", init_mem_usage, mem_usage-init_mem_usage, page_faults);
        }
        break;
    case EACH:
        for(int iteration = 1; iteration <= 10; iteration++) {
            for(int matrix_no = 0; matrix_no < 5; matrix_no++) {
                matrix[matrix_no] = (int**) malloc(n * sizeof(int*));
                for(int i = 0; i < n; i++)
                    matrix[matrix_no][i] = (int*) malloc(n * sizeof(int));
                for(int i = 0; i < n; i++)
                    for(int j = 0; j < n; j++)
                        matrix[matrix_no][i][j] = rand();
                transpose(matrix[matrix_no], n);
            }
            getrusage(RUSAGE_SELF, &process_usage);
            long page_faults = process_usage.ru_minflt;
            long mem_usage = process_usage.ru_maxrss;
            printf("memory usage: %ld + %ld, page_faults: %ld\n", init_mem_usage, mem_usage-init_mem_usage, page_faults);
        }
        break;
    }
    return 0;
}