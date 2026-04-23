#include <unistd.h> //For sleep() and getpid()
#include <stdio.h> //For printf()
#include <pthread.h>
#include <string.h>

int race;

void adder() {
    for (long long int i = 0; i < 20000000; i++) {
        race++;
    }
    printf("%d\n", race);
}

void subtractor() {
    for (long long int i = 0; i < 20000000; i++) {
        race--;
    }
    printf("%d\n", race);
}

void* thread_add(void* arg) {
    for (long long int i = 0; i < 20000000; i++) {
        race++;
    }
    printf("%d\n", race);
}

void* thread_sub(void* arg) {
    for (long long int i = 0; i < 20000000; i++) {
        race--;
    }
    printf("%d\n", race);
}

int main() {
    pthread_t thread1; 
    pthread_t thread2; 
    if (pthread_create(&thread1, NULL, thread_add, NULL) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    if (pthread_create(&thread2, NULL, thread_sub, NULL) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    if (pthread_join(thread1, NULL) != 0) {
            fprintf(stderr, "Error joining thread\n");
            return 1;
    }
    
    if (pthread_join(thread2, NULL) != 0) {
            fprintf(stderr, "Error joining thread\n");
            return 1;
    }
        
    // adder();
    // subtractor();
    return 0;
};