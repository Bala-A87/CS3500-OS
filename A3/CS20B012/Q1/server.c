#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#define MSG_SIZE 256

struct msg_buf {
    long msg_type;
    char msg_text[MSG_SIZE];
} message_snd, message_rcv;


// Converts long long int to char* for message passing
char* lltoa(long long int val) {
    if(val == 0) { return strdup("0"); }
    char *valString = strdup("");
    int isNegative = 0;
    if(val < 0) {
        isNegative = 1;
        val *= -1;
    }
    while(val) {
        int temp = val % 10;
        char tempVal = temp+'0';
        strncat(valString, &tempVal, 1);
        val /= 10;
    }
    if(isNegative) {
        char tempVal = '-';
        strncat(valString, &tempVal, 1);
    }
    int n = strlen(valString);
    int i = 0, j = n-1;
    while(i < j) {
        char temp = valString[i];
        valString[i] = valString[j];
        valString[j] = temp;
        i++;
        j--;
    }
    return valString;
}


int main() {
    // Creating the message queues
    int C2S = msgget(18462, 0666 | IPC_CREAT);
    int S2C = msgget(48236, 0666 | IPC_CREAT);
    message_snd.msg_type = 1;

    while(1) {
        msgrcv(C2S, &message_rcv, sizeof(message_rcv), 1, 0);
        printf("Received command %s from client\n", message_rcv.msg_text);
        
        char buf[256];
        int *cpu;
        long long retval;
        char invalid[20] = "Invalid system call";

        if(!strcmp(message_rcv.msg_text, "pwd"))
            strcpy(message_snd.msg_text, getcwd(buf, 256));

        else if(!strcmp(message_rcv.msg_text, "pidof")) {
            retval = getpid();
            strcpy(message_snd.msg_text, lltoa(retval));
        }

        else if(!strcmp(message_rcv.msg_text, "time")) {
            retval = time(NULL);
            strcpy(message_snd.msg_text, lltoa(retval));
        }

        else if(!strcmp(message_rcv.msg_text, "id")) {
            retval = getuid();
            strcpy(message_snd.msg_text, lltoa(retval));
        }

        else if(!strcmp(message_rcv.msg_text, "mkdir")) {
            retval = mkdir(strdup("./dummy-folder"), 0777);
            strcpy(message_snd.msg_text, lltoa(retval));
        }

        else if(!strcmp(message_rcv.msg_text, "End"))
            break;

        else
            strcpy(message_snd.msg_text, invalid);  

        printf("Output computed: %s\n", message_snd.msg_text);
        msgsnd(S2C, &message_snd, sizeof(message_snd), 0);
    }

    printf("Terminating...\n");
    msgctl(C2S, IPC_RMID, NULL);
    msgctl(S2C, IPC_RMID, NULL);

    return 0;
}