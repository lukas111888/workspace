#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// This is the thread function that executes when the thread is created
// it passes and receives data by void pointers
void *threadFunction(void *value){
    int *x = (int *)value; // cast the data passed to an int pointer
    while(*x<5){ // while the value of x is less than 5
        usleep(10); // sleep for 10us - encourage main thread
        (*x)++; // increment the value of x by 1
        printf("aaa");
    }
    return x; // return the pointer x (as a void*)
}
int main() {
    int x=0, y=0, z=0;
    pthread_t thread0; // this is our handle to the pthread
    // create the thread, pass the reference, address of the function and data
    // pthread_create() returns 0 on the successful creation of a thread
    if(pthread_create(&thread0, NULL, &threadFunction, &x)!=0){
        printf("Failed to create the thread" );
        return 1;
    }

    pthread_t thread1; // this is our handle to the pthread
    // create the thread, pass the reference, address of the function and data
    // pthread_create() returns 0 on the successful creation of a thread
    if(pthread_create(&thread1, NULL, &threadFunction, &z)!=0){
        printf("Failed to create the thread" );
        return 1;
    }
    // at this point the thread was created successfully
    while(y<5){ // loop and increment y, displaying values
        printf( "The value of x=%d and y=%d and z=%d\n",x, y++, z);
        usleep(10); // encourage the pthread to run
    }
    /**void* result; // OPTIONAL: receive data back from pthread
    pthread_join(thread, &result); // allow the pthread to complete
    int *z = (int *) result; // cast from void* to int* to get z
    **/
    printf("Final: x=%d y=%d and z=%d\n",x,y,z);
    return 0;
}