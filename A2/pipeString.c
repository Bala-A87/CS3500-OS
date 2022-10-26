#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>

int main() {
    int pipefd[2];
    if(pipe(pipefd))
        exit(-1);

    int pid = fork();
    if(pid < 0)
        exit(-1);

    else if(pid == 0) { //Child process
        char inStr[256];
        read(STDIN_FILENO, inStr, 256);
        close(pipefd[0]);
        write(pipefd[1], inStr, strlen(inStr));
        close(pipefd[1]);
    }

    else { //Parent process
        close(pipefd[1]);
        char childStr[256];
        read(pipefd[0], childStr, 256);
        close(pipefd[0]);
        printf("%s", childStr);
        wait(NULL);
    }

    return 0;
}