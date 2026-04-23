//Runlength compression in C
// Usage: ./rle <input file> <output file> <runlength> <mode>
//        Where mode=0 is compress and mode=1 is decompress

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void compress(int, int, int);
void decompress(int, int, int);

int main(int argc, char* argv[]) {
    if (argc != 5) {    // check for correct usge
        printf("Usage: ./rle <input file> <output file> <runlength> <mode>\n");
        exit(-1);
    }
    
    // store all arguments as variables
    char *inputFileName = argv[1];
    char *outputFileName = argv[2];
    int runLength = atoi(argv[3]);
    int mode = atoi(argv[4]);
    
    if (runLength < 1) {    // check for a correct runlength value
        printf("Argument <runlength> must be an int of at least 1\n");
        exit(-1);
    }
    if (mode != 0 && mode != 1) {   // check for a correct mode value
        printf("Argument <mode> must be either 0 or 1\n");
        exit(-1);
    }

    int inputFile = open(inputFileName, O_RDONLY);  // we only want to read from the input file
    int outputFile = open(outputFileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);   // we only want to write to the output file and give the user read and write permissions

    // check for errors in opening the input and output files
    if (inputFile == -1) {
        perror("Error opening input file");
        exit(-1);
    }
    if (outputFile == -1) {
        perror("Error opening output file");
        exit(-1);
    }

    if (mode == 0) {
        compress(inputFile, outputFile, runLength);
    } else {
        decompress(inputFile, outputFile, runLength);
    }

    close(inputFile);
    close(outputFile);

    return 0;
}

void compress(int inputFile, int outputFile, int runLength) {
    char buffer[runLength]; // the buffer will have <runlength> chars in it
    ssize_t bytesRead;
    char *prev = NULL;
    int counter = 0;
    
    while ((bytesRead = read(inputFile, buffer, runLength)) > 0) {
        if (prev == NULL || (memcmp(buffer, prev, runLength) == 0 && counter != 255)) {  // if we are at the start or the pattern repeats, add 1 to counter (use memcmp)
            counter++;
        } else if (memcmp(buffer, prev, runLength) != 0 || counter == 255){    // if we find a new pattern or we find 255 repetitions of a string, write how many times we found the previous pattern (also, i found that strncmp didn't work as well as memcmp for the slu_logo.bmp file, i think because the images aren't necessarily null-terminated)
            write(outputFile, &counter, 1);
            write(outputFile, prev, runLength);
            counter = 1;
        }
        if (bytesRead < runLength) {    // if we read less data than we want to, we found a sequence once
            counter = 1;
            write(outputFile, &counter, 1);
            write(outputFile, buffer, bytesRead);
            free(prev);
            prev = NULL;
            break;
        }

        if (prev != NULL) { // clear prev's memory if it can be cleared
            free(prev);
        }
        prev = (char*)calloc(bytesRead, sizeof(char));   // copy the buffer into prev
        for (int i = 0; i < bytesRead; i++) {
            prev[i] = buffer[i];
        }
    }

    if (prev != NULL) { // if prev hasn't been set to null, write the sequence to file
        write(outputFile, &counter, 1);
        write(outputFile, prev, runLength);
        free(prev);
    }
}

void decompress(int inputFile, int outputFile, int runLength) {
    char buffer[runLength+1]; // so we can read the int and the sequence
    ssize_t bytesRead;

    while ((bytesRead = read(inputFile, buffer, runLength+1)) > 0) {
        int count = (unsigned char)buffer[0];
        char* temp = (char*)calloc(bytesRead-1, sizeof(char));
        for (int i = 1; i < bytesRead; i++) {
            temp[i-1] = buffer[i];
        }
        for (int i = 0; i < count; i++) {
            write(outputFile, temp, bytesRead-1);
        }
        free(temp);
    }
}