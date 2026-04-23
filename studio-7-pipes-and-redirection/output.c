#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



int main() {
    while(1) {
        char input[50];
        printf("Enter a string: ");

        while(1) {
            if(fgets(input, sizeof(input), stdin) != NULL){
                break;
            }
        }
        //print statement
        printf("Program 2 got: %s\n", input);
    }
    return 0;
}