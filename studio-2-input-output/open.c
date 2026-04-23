//Write a program that accepts a single command line argument
//	the argument should be interpreted as a file name that
//	your program attempts to open. once this file is opened,
//	your program is should print the contents of the file to
//	standard output. i.e. "./open mars.txt" should open the
//	mars.txt and print it to the console. Use the "open()"
//	function documented at "man 2 open".

// Hint: modify copy.c to opne a file and then modify the "read()"
//	statement to read from your file descriptor rather than
//	standard input.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> // for printf
#include <stdlib.h>

#define bufferSize 200

int main(int argc, char *argv[]) {

        // only one parameter
        if (argc != 2) {
                printf("may only be one parameter\n");
                return(-1);
        }

        //open the file
        int inputFile = open(argv[1], O_RDONLY, O_CREAT);    // get the file and make it read only

        if (inputFile == -1) {
                perror("Error opening input file");
                exit(-1);
        }

        //initialize buffer
        char buffer[ bufferSize ];
        int numBytes;

        //read in until carrige return, then call break
        while(1) {

        numBytes = read(inputFile, buffer, bufferSize);

        if (numBytes == 0) {
                break;
        }

        //print contents to standard output
        write(1, buffer, numBytes);
        }

        close(inputFile); //close input file
        return 0;
}

