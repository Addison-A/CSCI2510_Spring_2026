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
    pthread_mutex_t* mutex = (pthread_mutex_t*)arg;
    pthread_mutex_lock(mutex);
    for (long long int i = 0; i < 20000000; i++) {
        race++;
    }
    pthread_mutex_unlock(mutex);
    printf("thread_add: %d\n", race);
    return NULL;
}

void* thread_sub(void* arg) {
    pthread_mutex_t* mutex = (pthread_mutex_t*)arg;
    pthread_mutex_lock(mutex);
    for (long long int i = 0; i < 20000000; i++) {
        race--;
    }
    pthread_mutex_unlock(mutex);
    printf("thread_sub: %d\n", race);
    return NULL;
}

int main() {
    //mutex initializer
    pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
    
    //pthread initializer
    pthread_t thread1; 
    pthread_t thread2; 
    
    if (pthread_create(&thread1, NULL, thread_add, &mutex1) != 0) {
        fprintf(stderr, "Error creating thread\n"); 
        return 1;
    }

    if (pthread_create(&thread2, NULL, thread_sub, &mutex1) != 0) {
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

    return 0;
};