#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<sys/stat.h>

int main()
{
    int opcode; 
    int count; //Number of commands seen, for history
    char cwd[PATH_MAX]; //Used to get current working directory
    getcwd(cwd, sizeof(cwd));
    struct stat s = {0}; //Used to check whether 'dir' exists
    int rmval; //Used to check if 'dir' was removed

    /*
    Commands:
    0 - clear
    1 - pwd
    2 - mkdir
    3 - rmdir
    4 - ls
    5 - ls -l
    6 - history
    7 - exit
    */

    do{
        //Get the command as input
        printf("%s> ", cwd);
        char op[10];
        fgets(op, 10, stdin); //reads /n too
        for(int i = 0; i < 10; i++)
            if(op[i] == '\n')
            {
                op[i] = 0;
                break;
            }

        //Obtain the opcode
        if(!strcmp(op, "clear"))
            opcode = 0;
        else if(!strcmp(op, "pwd"))
            opcode = 1;
        else if(!strcmp(op, "mkdir"))
            opcode = 2;
        else if(!strcmp(op, "rmdir"))
            opcode = 3;
        else if(!strcmp(op, "ls"))
            opcode = 4;
        else if(!strcmp(op, "ls -l"))
            opcode = 5;
        else if(!strcmp(op, "history"))
            opcode = 6;
        else if(!strcmp(op, "exit"))
            opcode = 7;
        else 
            opcode = -1;

        //Enter command into history
        FILE *ptr = fopen("commands.txt", "a");
        fprintf(ptr, "%s\n", op);
        fclose(ptr);
        
        switch(opcode)
        {
        case 0: //clear
            system("clear");
            break;

        case 1: //pwd
            printf("%s\n", cwd);
            break;

        case 2: //mkdir
            if(stat("dir", &s) == -1)
                mkdir("dir", 0777);
            else
                printf("mkdir: cannot create directory 'dir': File exists\n");
            break;

        case 3: //rmdir
            rmval = rmdir("dir");
            if(rmval != 0)
                printf("rmdir: failed to remove 'dir'\n");
            break;

        case 4: //ls
            system("ls");
            break;

        case 5: //ls -l
            system("ls -l");
            break;

        case 6: //history
            ptr = fopen("commands.txt", "r");
            char nextop[15];
            count = 1;
            while(fgets(nextop, sizeof(nextop), ptr)){
                printf("%d %s", count, nextop);
                count++;
            }
            fclose(ptr);
            break;

        case 7: //exit
            exit(0);
            break;

        default: //invalid command
            printf("Command %s not found\n", op);
        }


    }while(opcode != 7);

    return 0;
}
