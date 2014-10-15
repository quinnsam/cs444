/******************************************************************************
 * Concurrency Project #1
 * CS444
 * Authors: Sam Quinn, ..
 * 10/14/2014
 ******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lock;

struct Data {
    int print;
    int wait_time;
};

struct Data product[32];

int check_mutex(){
    return pthread_mutex_trylock(&lock);
}

int check_empty(int index){
    int rv = 0;

    if (product[index].wait_time == -1 || product[index].print == -1) {
        rv = 1;
    } 
    return rv;
}
/* this function is run by the second thread */
void consumer()
{
    int i, time;
    while (1){
        if (i >= 32 ) {
            i = 0;
        }
        if (!check_mutex()){
            printf("Mutex Aquired\n");
            if (!check_empty(i)) {
                printf("Print: %d\n", product[i].print);
                printf("Wait: %d\n", product[i].wait_time);
                time = product[i].wait_time;
                product[i].print = -1;
                product[i].wait_time = -1;
                sleep(time);
            }
            pthread_mutex_unlock(&lock);
            printf("Mutex released\n");
            
        } else {
            printf("Mutex not aquired.\n");
        }
        i++;
    }
}


int main()
{
    int i;
    
    for (i = 0; i < 32; ++i){
        product[i].print=i;
        product[i].wait_time=i+5;
    }
    
    //Prints all the data in the array.
    for (i = 0; i < 32; ++i){
        printf("Print: %d\n", product[i].print);
        printf("Wait: %d\n", product[i].wait_time);
    }

    /* this variable is our reference to the second thread */
    pthread_t consumer_thread;

    /* create a second thread which executes inc_x(&x) */
    if(pthread_create(&consumer_thread, NULL, (void *)consumer, (void *)NULL)) {

        fprintf(stderr, "Error creating thread\n");
        return 1;

    }

    /* wait for the second thread to finish */
    if(pthread_join(consumer_thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    return 0;

}
