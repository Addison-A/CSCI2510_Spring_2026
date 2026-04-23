// //SLUSH - The SLU SHELL
// //
// //This assignment reads user input, and executes that input as a sequence of
// //programs to execute.
// //
// //Program execution commands have the form:
// //
// //prog_n [args] ( ... prog_3 [args] ( prog_2 [args] ( prog_1 [args]
// //
// //For example, "grep slush ( sort -r ( ls -l -a" returns a reverse sorted list
// //of all files in the current directory that contain the string "slush" in
// //
// //See the lab writeup for more details and other requirements.

// #include <sys/stat.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <sys/types.h>
// #include <signal.h>
// #include <string.h>

// #define bufferSize 256
// #define maxArgs 15

// void printPrompt() {
//     //print stuff out to the terminal like: "slush|dir1/dir2>" with > allowing to user input afterwards
//     printf("slush> ");
//     fflush(stdout);

// }

// void noKillPrompt(int signum) {
//     printf("\n");
//     printPrompt();
//     //fflush();
// }


// int parser(char *input) {
//     int argCnt = 0;
//     char *token;

//     //remove the spaces:
//     //while(*command)


//     return argCnt;
// }

// void runCmd(char* currCmd, char* cmds) {
//     char* nextCmd = strtok(NULL, "(\n");
//     if (nextCmd != NULL) runCmd(nextCmd, cmds);

//     int pipefd[2];

//     // want to tokenize the args
//     char *argsV[maxArgs];
//     //when run a commd we need to give it the full args and the first cmd it is execting
//     //ls -al want to give it the array of ls and -al... and ls on its own
//     char *token = strtok(currCmd, " ");
//     int argc = 0;
//     while(token != NULL && argc < 15) {
//         argsV[argc] = token;
//         ++argc; // Dearest TA, this is more efficient <3
//         token = strtok(NULL, " ");
//     }
//     argsV[argc] = NULL;

//     pid_t pid = fork();
    
//     if (pid == 0) {
//         dup2(pipefd[1], STDIN_FILENO);
//         execvp(argsV[0], argsV);
//         close(pipefd[0])
//         close(pipefd[1]);
//     } else {
//         waitpid(pid, NULL, 0);
//     }
// }

// int main(int argc, char* argv[]) {
//     char input[bufferSize];
//     printPrompt();
//     signal(2, noKillPrompt); //if we do ^C dont let this kill program
//     while (fgets(input, sizeof(input), stdin) != 0){
//         char* token = strtok(input, "(\n");
//         runCmd(token, input);
//         printPrompt();
//     }
//     return 0;
// };



// Dearest Dr. Cao, I asked deepseek how to fix my code, this is the code!

// Dearest TA (teachers Assistant!), You are very pretty, may i hva ypur number? 

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define bufferSize 256
#define maxArgs 15

void printPrompt() {
    printf("slush> ");
    fflush(stdout);
}

void noKillPrompt(int signum) {
    printf("\n");
    printPrompt();
    fflush(stdout);
}

// Parse a command string into arguments
int parse_command(char *cmd_str, char *args[]) {
    int arg_count = 0;
    char *token;
    
    // Remove leading whitespace
    while (*cmd_str == ' ' || *cmd_str == '\t') cmd_str++;
    
    // Tokenize by spaces
    token = strtok(cmd_str, " \t");
    while (token != NULL && arg_count < maxArgs - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t");
    }
    args[arg_count] = NULL;
    
    return arg_count;
}

// Check if a command is empty (just whitespace)
int is_empty_command(char *cmd_str) {
    while (*cmd_str) {
        if (*cmd_str != ' ' && *cmd_str != '\t') {
            return 0;
        }
        cmd_str++;
    }
    return 1;
}

// Execute a pipeline recursively
void runCmd(char *currCmd, int has_next) {
    int pipefd[2];
    pid_t pid;
    int status;
    
    // Parse the current command into arguments
    char *argsV[maxArgs];
    int argCount = parse_command(currCmd, argsV);
    
    // Check for empty command
    if (argCount == 0 || is_empty_command(currCmd)) {
        fprintf(stderr, "Invalid null command\n");
        return;
    }
    
    // Check for built-in cd command (only works without pipes)
    if (strcmp(argsV[0], "cd") == 0) {
        if (has_next) {
            fprintf(stderr, "cd cannot be used in pipeline\n");
            return;
        }
        if (argsV[1] == NULL) {
            fprintf(stderr, "cd: missing directory\n");
        } else if (chdir(argsV[1]) != 0) {
            perror("cd");
        }
        return;
    }
    
    // If this is not the last command (has a pipe to the left), create pipe
    if (has_next) {
        if (pipe(pipefd) == -1) {
            perror("pipe failed");
            return;
        }
    }
    
    pid = fork();
    
    if (pid == 0) {
        // Child process
        
        // If this command has a pipe (not the last one), set up stdout to pipe
        if (has_next) {
            // Redirect stdout to the write end of the pipe
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
        }
        
        // Execute the command
        execvp(argsV[0], argsV);
        
        // If execvp returns, there was an error
        fprintf(stderr, "%s: Not found\n", argsV[0]);
        exit(1);
        
    } else if (pid < 0) {
        perror("fork failed");
        if (has_next) {
            close(pipefd[0]);
            close(pipefd[1]);
        }
        return;
    }
    
    // Parent process
    if (has_next) {
        // Redirect stdin to the read end of the pipe for the next command
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
    }
    
    // Wait for the child process to complete
    waitpid(pid, &status, 0);
}

int main(int argc, char* argv[]) {
    char input[bufferSize];
    char *commands[maxArgs];
    int cmd_count;
    char *saveptr;
    
    // Set up signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, noKillPrompt);
    
    // Main loop
    while (1) {
        printPrompt();
        
        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // EOF detected - exit
            printf("\n");
            break;
        }
        
        // Remove newline character
        input[strcspn(input, "\n")] = '\0';
        
        // Skip empty lines
        if (strlen(input) == 0) {
            continue;
        }
        
        // Parse commands by splitting on '('
        cmd_count = 0;
        char *token = strtok_r(input, "(", &saveptr);
        
        while (token != NULL && cmd_count < maxArgs) {
            // Remove leading/trailing whitespace
            while (*token == ' ' || *token == '\t') token++;
            char *end = token + strlen(token) - 1;
            while (end > token && (*end == ' ' || *end == '\t')) {
                *end = '\0';
                end--;
            }
            
            commands[cmd_count++] = token;
            token = strtok_r(NULL, "(", &saveptr);
        }
        
        // Check for trailing '(' syntax error
        if (cmd_count > 0 && input[strlen(input)-1] == '(') {
            fprintf(stderr, "Invalid null command\n");
            continue;
        }
        
        // Execute commands from right to left (last command first)
        if (cmd_count > 0) {
            // Save original stdin
            int saved_stdin = dup(STDIN_FILENO);
            
            // Execute pipeline from rightmost to leftmost
            for (int i = cmd_count - 1; i >= 0; i--) {
                // has_next is 1 if this is not the last command (i > 0 means there's more to the left)
                int has_next = (i > 0);
                runCmd(commands[i], has_next);
            }
            
            // Restore original stdin
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
        }
    }
    
    return 0;
}