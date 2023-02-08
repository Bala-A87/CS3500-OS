#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

// Integer equivalents for the scheduling algorithms, for usage in code

#define UNINITIALIZED -1
#define FCFS 1
#define SSTF 2
#define SCAN 3
#define CSCAN 4
#define LOOK 5
#define CLOOK 6

// Specified values for disk size, number of requests and default initial head position
// May be modified by user if required

const int HEAD_MAX = 5000;
const int NO_REQUESTS = 1000;
const int DEFUALT_INIT_HEAD = 2000;

// Specifications, to be shown during incorrect usage
void show_usage() {
    printf("Usage: ./disk-scheduler -h <head position> -a <scheduling algorithm>\n");
    printf("Head position is an optional argument, with default value %d\n", DEFUALT_INIT_HEAD);
    printf("The two arguments may be interchanged, i.e., head position may be specified first, last or not at all\n");
    printf("As long as the flags -a and -h are specified before each argument to inform which argument is which\n");
    printf("Valid values of head position: Integer between 0 and %d, both inclusive\n", HEAD_MAX-1);
    printf("Valid scheduling algorithms:\n");
    printf("\tFCFS\n\tSSTF\n\tSCAN\n\tCSCAN\n\tLOOK\n\tCLOOK\n");
    exit(1);
}

// Generates random request sequence
void get_request_sequence(int *requests) {
    for(int i = 0; i < NO_REQUESTS; i++)
        requests[i] = rand()%HEAD_MAX;
}

// Computes head movement between head and target
int compute_head_movement(int head, int target) {
    if(head > target) return head-target;
    return target-head;
}

// Swaps values in an array for the specified pair of indices
void swap(int *arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// Comparator for qsort
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}


int main(int argc, char* argv[]) {
    srand(time(0));
    int head = DEFUALT_INIT_HEAD;
    int algorithm = UNINITIALIZED;
    int *requests = (int*) malloc(NO_REQUESTS * sizeof(int));

    // Extract cmd line args, generate access sequence
    if(argc < 2) {
        show_usage();
    }
    else {
        for(int i = 1; i < argc; i++)
            if(!strcmp(argv[i], "-h")) {
                head = atoi(argv[i+1]);
                if(head < 0 || head >= HEAD_MAX) {
                    printf("Invalid head position, please adhere to specifications\n");
                    show_usage();
                }
                i++;
            }
            else if(!strcmp(argv[i], "-a")) {
                i++;
                if(!strcmp("FCFS", argv[i])) algorithm = FCFS;
                else if(!strcmp("SSTF", argv[i])) algorithm = SSTF;
                else if(!strcmp("SCAN", argv[i])) algorithm = SCAN;
                else if(!strcmp("CSCAN", argv[i])) algorithm = CSCAN;
                else if(!strcmp("LOOK", argv[i])) algorithm = LOOK;
                else if(!strcmp("CLOOK", argv[i])) algorithm = CLOOK;
                else {
                    printf("Invalid scheduling algorithm, please adhere to specifications\n");
                    show_usage();
                }
            }
    }
    if(algorithm == UNINITIALIZED) {
        printf("Compulsory argument (scheduling algorithm) missing, please adhere to specifications\n");
        show_usage();
    }
    get_request_sequence(requests);
    printf("Request sequence:\n");
    for(int i = 0; i < NO_REQUESTS; i++) printf("%d ", requests[i]);

    int total_head_movement = 0;
    int count_lt_head = 0, count_gt_head = 0;
    int *lt_head, *gt_head;
    int lt_used, gt_used;

    printf("\n\nService sequence:\n");
    
    switch(algorithm) {
    case FCFS:
        for(int i = 0; i < NO_REQUESTS; i++) {
            total_head_movement += compute_head_movement(head, requests[i]);
            head = requests[i];
            printf("%d ", head);
        }
        break;

    case SSTF:
        for(int i = 0; i < NO_REQUESTS; i++) {
            int sst_index = i;
            for(int j = i+1; j < NO_REQUESTS; j++) {
                // In every iteration, "given" initial head position, we find the request closest to it
                int best_dist = compute_head_movement(head, requests[sst_index]);
                int curr_dist = compute_head_movement(head, requests[j]);
                if(curr_dist < best_dist) sst_index = j;
            }
            // Swap the best index into the current index reached in requests, so that we don't have to check again if a request was serviced
            swap(requests, i, sst_index);
            total_head_movement += compute_head_movement(head, requests[i]);
            head = requests[i];
            printf("%d ", head);
        }
        break;

    case SCAN:
        // Start by separating the requests which occur below initial head and above it
        for(int i = 0; i < NO_REQUESTS; i++) {
            if(requests[i] <= head) count_lt_head++;
            else if(requests[i] > head) count_gt_head++;
        }
        lt_head = (int*) malloc(count_lt_head * sizeof(int));
        gt_head = (int*) malloc(count_gt_head * sizeof(int));
        lt_used = 0;
        gt_used = 0;
        for(int i = 0; i < NO_REQUESTS; i++)
            if(requests[i] <= head) lt_head[lt_used++] = requests[i];
            else if(requests[i] > head) gt_head[gt_used++] = requests[i];
        qsort(lt_head, count_lt_head, sizeof(int), cmpfunc);
        qsort(gt_head, count_gt_head, sizeof(int), cmpfunc);

        // Move towards the closer end of the disk, then the other way
        if(head >= HEAD_MAX/2) {
            for(int i = 0; i < count_gt_head; i++) {
                total_head_movement += compute_head_movement(head, gt_head[i]);
                head = gt_head[i];
                printf("%d ", head);
            }
            // If there are no requests at positions less than the initial head, the jump to the end of the disk and turning around are not needed
            if(count_lt_head > 0) {
                total_head_movement += compute_head_movement(head, HEAD_MAX-1);
                for(int i = count_lt_head-1; i >= 0; i--) {
                    total_head_movement += compute_head_movement(head, lt_head[i]);
                    head = lt_head[i];
                    printf("%d ", head);
                }
            }
        }
        else {
            for(int i = count_lt_head-1; i >= 0; i--) {
                total_head_movement += compute_head_movement(head, lt_head[i]);
                head = lt_head[i];
                printf("%d ", head);
            }
            if(count_gt_head > 0) {
                total_head_movement += compute_head_movement(head, 0);
                head = 0;
                for(int i = 0; i < count_gt_head; i++) {
                    total_head_movement += compute_head_movement(head, gt_head[i]);
                    head = gt_head[i];
                    printf("%d ", head);
                }
            }
        }

        free(lt_head);
        free(gt_head);
        break;
    
    case CSCAN:
        for(int i = 0; i < NO_REQUESTS; i++) {
            if(requests[i] <= head) count_lt_head++;
            else if(requests[i] > head) count_gt_head++;
        }
        lt_head = (int*) malloc(count_lt_head * sizeof(int));
        gt_head = (int*) malloc(count_gt_head * sizeof(int));
        lt_used = 0;
        gt_used = 0;
        for(int i = 0; i < NO_REQUESTS; i++)
            if(requests[i] <= head) lt_head[lt_used++] = requests[i];
            else if(requests[i] > head) gt_head[gt_used++] = requests[i];
        qsort(lt_head, count_lt_head, sizeof(int), cmpfunc);
        qsort(gt_head, count_gt_head, sizeof(int), cmpfunc);

        if(head >= HEAD_MAX/2) {
            for(int i = 0; i < count_gt_head; i++) {
                total_head_movement += compute_head_movement(head, gt_head[i]);
                head = gt_head[i];
                printf("%d ", head);
            }
            if(count_lt_head > 0) {
                total_head_movement += compute_head_movement(head, HEAD_MAX-1) + HEAD_MAX;
                head = 0;
                for(int i = 0; i < count_lt_head; i++) {
                    total_head_movement += compute_head_movement(head, lt_head[i]);
                    head = lt_head[i];
                    printf("%d ", head);
                }
            }
        }
        else {
            for(int i = count_lt_head-1; i >= 0; i--) {
                total_head_movement += compute_head_movement(head, lt_head[i]);
                head = lt_head[i];
                printf("%d ", head);
            }
            if(count_gt_head > 0) {
                total_head_movement += compute_head_movement(head, 0) + HEAD_MAX;
                head = HEAD_MAX-1;
                for(int i = count_gt_head-1; i >= 0; i--) {
                    total_head_movement += compute_head_movement(head, gt_head[i]);
                    head = gt_head[i];
                    printf("%d ", head);
                }
            }
        }

        free(lt_head);
        free(gt_head);
        break;

    case LOOK:
        for(int i = 0; i < NO_REQUESTS; i++) {
            if(requests[i] <= head) count_lt_head++;
            else if(requests[i] > head) count_gt_head++;
        }
        lt_head = (int*) malloc(count_lt_head * sizeof(int));
        gt_head = (int*) malloc(count_gt_head * sizeof(int));
        lt_used = 0;
        gt_used = 0;
        for(int i = 0; i < NO_REQUESTS; i++)
            if(requests[i] <= head) lt_head[lt_used++] = requests[i];
            else if(requests[i] > head) gt_head[gt_used++] = requests[i];
        qsort(lt_head, count_lt_head, sizeof(int), cmpfunc);
        qsort(gt_head, count_gt_head, sizeof(int), cmpfunc);

        if(head >= HEAD_MAX/2) {
            for(int i = 0; i < count_gt_head; i++) {
                total_head_movement += compute_head_movement(head, gt_head[i]);
                head = gt_head[i];
                printf("%d ", head);
            }
            for(int i = count_lt_head-1; i >= 0; i--) {
                total_head_movement += compute_head_movement(head, lt_head[i]);
                head = lt_head[i];
                printf("%d ", head);
            }
        }
        else {
            for(int i = count_lt_head-1; i >= 0; i--) {
                total_head_movement += compute_head_movement(head, lt_head[i]);
                head = lt_head[i];
                printf("%d ", head);
            }
            for(int i = 0; i < count_gt_head; i++) {
                total_head_movement += compute_head_movement(head, gt_head[i]);
                head = gt_head[i];
                printf("%d ", head);
            }
        }

        free(lt_head);
        free(gt_head);
        break;

    case CLOOK:
        for(int i = 0; i < NO_REQUESTS; i++) {
            if(requests[i] <= head) count_lt_head++;
            else if(requests[i] > head) count_gt_head++;
        }
        lt_head = (int*) malloc(count_lt_head * sizeof(int));
        gt_head = (int*) malloc(count_gt_head * sizeof(int));
        lt_used = 0;
        gt_used = 0;
        for(int i = 0; i < NO_REQUESTS; i++)
            if(requests[i] <= head) lt_head[lt_used++] = requests[i];
            else if(requests[i] > head) gt_head[gt_used++] = requests[i];
        qsort(lt_head, count_lt_head, sizeof(int), cmpfunc);
        qsort(gt_head, count_gt_head, sizeof(int), cmpfunc);

        if(head >= HEAD_MAX/2) {
            for(int i = 0; i < count_gt_head; i++) {
                total_head_movement += compute_head_movement(head, gt_head[i]);
                head = gt_head[i];
                printf("%d ", head);
            }
            for(int i = 0; i < count_lt_head; i++) {
                total_head_movement += compute_head_movement(head, lt_head[i]);
                head = lt_head[i];
                printf("%d ", head);
            }
        }
        else {
            for(int i = count_lt_head-1; i >= 0; i--) {
                total_head_movement += compute_head_movement(head, lt_head[i]);
                head = lt_head[i];
                printf("%d ", head);
            }
            for(int i = count_gt_head-1; i >= 0; i--) {
                total_head_movement += compute_head_movement(head, gt_head[i]);
                head = gt_head[i];
                printf("%d ", head);
            }
        }

        free(lt_head);
        free(gt_head);
        break;
    }

    printf("\nAverage head movement: %.3lf\n", (double)(total_head_movement)/NO_REQUESTS);
    printf("Total head movement: %d\n", total_head_movement);

    free(requests);

    return 0;
}