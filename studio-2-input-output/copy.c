//Replace this file with your copy.c
//header file to include teh read() and write() system calls: #include <unistd.h>
#include <unistd.h>
#define bufferSize 200

int main() {
	char buffer[ bufferSize ];
	int numBytes = 0;
	//read in until carrige return, then call break
	while(1){
	numBytes = read(STDIN_FILENO, buffer, bufferSize);
	
	if (numBytes == 0) {
	break;
	}
	write(STDOUT_FILENO, buffer, numBytes);
	//write contents of buffer to (STDOUT_FILENO), using
	//   the return value from read()
	}
	return 0;
}
