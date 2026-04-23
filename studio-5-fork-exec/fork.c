#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> // for printf
#include <stdlib.h>

#define bufferSize 200

int main(int argc, char *argv[]) {

    //only the command
    if (argc != 1) {
            printf("no parameters accepted\n");
            return(-1);
    }

    // creat a child process by recalling fork
    int childPID = fork();

    if (childPID == 0) {   //print the child process
        //sleep(3);
        waitpid( childPID, NULL, 0 );
        printf("Child PID is: %d\n", getpid());
        char* child_argv[] = {"ls", "-l", NULL};
        char* child_prog = child_argv[0];
        execvp(child_prog, child_argv);
        waitpid(child_prog, NULL, 0 );
        printf("Child PID is: %d\n", getpid());
        //char* args[] = {"ls", "-l", NULL}; //Execute the "ls -l" command
        
    } else if (childPID > 0) {
        printf("Parent PID is: %d\n", getpid());
    }
    
    //the memory for each process is private to that process
    // cannot directly modify the mem of parent process through the child process

    //printf("Parent PID is: %d\n", getpid());
    return 0;
};