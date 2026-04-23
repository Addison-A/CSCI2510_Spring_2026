//Runlength compression in C
// Usage: ./rle <input file> <output file> <runlength> <mode>
//        Where mode=0 is compress and mode=1 is decompress

#include <stdio.h> // for printf
#include <unistd.h> //so we have the read, write, and close system calls
//include statements for open "man 2 open" & "man 2 close"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//lib for atoi
#include <stdlib.h>

//lib for memcpy
#include <string.h>


#define bufferSize 200 // buffer to hold files

#define baseCounter 8 //make 8-byte counter to store values 0-255

// given the run-length parameter, K, and starting at the beginning of the input file:
// 1. read the first byte of the file as an integer, N
// 2. Scan the next set of K bytes in the file...
//    if these bytes equal the last set of K bytes, increment counter by 1.
// 3. Repeat the prev step until a new pattern is found.
// 4. when pattern is found, output the counter's value along with the original pattern.
// 5. Reset counter to 1 and repeat the above process for new pattern by going to step 2.

//rle <input file> <output file> <compression length> <mode>

int main(int argc, char *argv[]) {

    //check that inputs are correct
    if (argc != 5) {
        write(STDERR_FILENO, "Usage: ./rle <input file> <output file> <runlength> <mode>\n", 65); // can also use printf
        exit(-1);
    }
    
    //Components from argument vector
    int K = atoi(argv[3]); // Set K to ASCII to integer
    int inputFile = open(argv[1], O_RDONLY);    // get the file and make it read only
    int outputFile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);    //open output file for writing only, create if it doesn't exist, truncate if it does exist
    int mode = atoi(argv[4]);   //ASCII to Integer 0: compress; 1: decompress

    // check for errors in accessing the input and output files
    if (inputFile == -1) {
        perror("Error opening input file");
        exit(-1);
    }
    if (outputFile == -1) {
        perror("Error opening output file");
        exit(-1);
    }

    //set up buffer; counter to keep track of bytes; counter for RLE
    char buffer[ bufferSize ];
    char *prev = NULL; //pointer to NULL to hold previous buffer value
	int numBytes;
    int counter = 0;

    // **** set mode **** 
    if (mode == 0) {            //compress
        while ((numBytes = read(inputFile, buffer, K)) > 0) {
            
            if (prev == NULL || (memcmp(prev, buffer, numBytes) == 0) && (counter < 255)) {
                counter++;
            } else if (memcmp(prev, buffer, numBytes) != 0 || counter == 255 ){
                write(outputFile, &counter, 1);     //write count- max of 8 bytes 0-255
                write(outputFile, prev, numBytes);   //write patterns
                counter = 1;
            }
            if (numBytes < K) {    // if we read less data than we want to, we found a sequence once
                counter = 1;
                write(outputFile, &counter, 1);
                write(outputFile, buffer, numBytes);
                free(prev);
                prev = NULL;
                break;
            }
            if (prev != NULL) {
                free(prev);
            }
            prev = (char*)calloc(numBytes, sizeof(char));   // copy the buffer into prev
            for (int i = 0; i < numBytes; i++) {
                prev[i] = buffer[i];
            }
        }
        if (prev != NULL) {
            write(outputFile, &counter, 1);
            write(outputFile, prev, K);
            free(prev);
        }
        
    } else if (mode == 1) { //decompress
        char decompress_buffer[K+1];

        while ((numBytes = read(inputFile, decompress_buffer, K+1)) > 0) {
            int N = (unsigned char)decompress_buffer[0];
            char* pattern = (char*)calloc(numBytes-1, sizeof(char));
            for (int i = 1; i < numBytes; i++) {
                pattern[i-1] = decompress_buffer[i];
            }
            for (int i = 0; i < N; i++) {
                write(outputFile, pattern, numBytes-1);
            }
            free(pattern);
            pattern = NULL;
        }
    } else {
        perror("ERROR: mode must be 0 for copress, or 1 for decompress\n");
        exit(-1);
    }

    close(inputFile); //close input file
    close(outputFile); //close output file

    return 0;
}