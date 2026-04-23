#include <unistd.h> //For sleep() and getpid()
#include <stdio.h> //For printf()
#include <stdlib.h> //For exit()
#include <signal.h>

int main(int argc, char* argv[]) {
    int* signalNum = 2; // get the signal number
    signal(signalNum, interrupter(signalNum)); //call the interrupter
    return 0;
};

void interrupter(int signum) {
    printf("We are ignoring SIGINT"); // print statement
    signal(signum, SIG_IGN);
};