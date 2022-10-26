#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#define MSG_SIZE 256

struct msg_buf {
    long msg_type;
    char msg_text[MSG_SIZE];
} message_snd, message_rcv;

int main() {
    int C2S = msgget(18462, 0666 | IPC_CREAT);
    int S2C = msgget(48236, 0666 | IPC_CREAT);
    message_snd.msg_type = 1;

    printf("Available system calls:\n");
    printf("\tpwd  : Prints current working directory\n");
    printf("\tpidof: Prints process id of the running executable\n");
    printf("\ttime : Prints the time in seconds since the epoch\n");
    printf("\tid   : Prints the id of the user\n");
    printf("\tmkdir: Makes a new directory called 'dummy-folder'\n");
    printf("\tEnd  : Terminates client and server\n");

    while(1) {
        printf("Enter the system call: ");
        fgets(message_snd.msg_text, MSG_SIZE, stdin);
        message_snd.msg_text[strlen(message_snd.msg_text)-1] = 0;
        msgsnd(C2S, &message_snd, sizeof(message_snd), 0);
        printf("Sending request to server\n");
        
        if(!strcmp(message_snd.msg_text, "End"))
            break;

        msgrcv(S2C, &message_rcv, sizeof(message_rcv), 1, 0);
        printf("Output received: %s\n", message_rcv.msg_text);
    }

    printf("Terminating...\n");
    msgctl(C2S, IPC_RMID, NULL);
    msgctl(S2C, IPC_RMID, NULL);

    return 0;
}
