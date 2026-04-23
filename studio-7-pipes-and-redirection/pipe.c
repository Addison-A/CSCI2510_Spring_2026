#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
//int main() {
    
    //Step 1: Make the Pipe
    int pipefd[2];
    int pid = pipe( pipefd );
    //pid_t process1 = fork();
    if (pid == -1) {
        perror("Could not open the pipe");
        return 1;
    } else if(pid == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        char* cmd = "./input"; 
        char* myargv[] = {"input", '\0'}; 
        int ret = execvp( cmd, myargv ); 
        if( ret == -1 ) {
            perror("Error exec'ing input");
        }
        close(pipefd[0]);
        close(pipefd[1]);
    } else {
        waitpid(pid, NULL, 0);
        close(pipefd[1]);
    }

    pid = fork();
    if(pid < 0){
        perror("Fork has failed");
        return 1;
    } else if (pid == 0){
        dup2(pipefd[0], STDIN_FILENO);
        char* cmd = "./output"; 
        char* myargv[] = {"output", '\0'};
        int ret = execvp( cmd, myargv ); 
        if( ret == -1 ) {
            perror("Error exec'ing output");
        }
        close(pipefd[0]);
        close(pipefd[1]);
    } else {
        waitpid(pid, NULL, 0);
        close(pipefd[1]);
    }

    return 0;
}