#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define num_threads 5

//Struct Definition
struct thread_args{int arg1; char arg2[100]; int ret;};

//thread entry
void *thread_entry(void *args) {
    //printf("Hello from the new thread!\n");
    
    // cast teh void * back to ptr
    struct thread_args *arg_ptr = (struct thread_args *) args;

    //print the vals
    // printf("%s\n", arg_ptr -> arg2);
    printf("thread arg1: %d\n", arg_ptr -> arg1);
    //printf("thread arg2: %s\n", arg_ptr -> arg2);
    
    
    // write a ret value
    arg_ptr -> ret = arg_ptr -> arg1 * 2;
    //printf("thread ret: %d\n", arg_ptr -> ret);

    return NULL;
}



int main() {
    pthread_t threads[num_threads];

    //give struct values
    struct thread_args args[num_threads];
    // args.arg1 = 100;
    // snprintf(args.arg2, sizeof(args.arg2), "Hello from main!");

    //create N threads
    printf("Creating %d threads \n", num_threads);
    for (int i = 0; i < num_threads; i++) {
        args[i].arg1 = i + 1;
        snprintf(args[i].arg2, sizeof(args[i].arg2), "Thread#: %d", i);
        
        if (pthread_create(&threads[i], NULL, thread_entry, &args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }
    // Create thread with default attributes and no arguments

    // Wait for the threads to finish
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 1;
        }
        printf("thread %d returned %d\n", i, args[i].ret);
    }

    //printf("Main thread returned: %d\n", args[i].ret);
    return 0;
};