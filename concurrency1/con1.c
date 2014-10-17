/******************************************************************************
 * Concurrency Project #1
 * CS444
 * Authors: Sam Quinn, ..
 * 10/14/2014
 ******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "rand.h"

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

        time = 0;
        
        if (!check_mutex()){
            //printf("Mutex Aquired by Consumer\n");
            if (!check_empty(i)) {
                printf("Consumer Printing from Index %d: %d\n", i, product[i].print);
                printf("Consumer Waiting at Index %d: %d\n", i, product[i].wait_time);
                time = product[i].wait_time;
                product[i].print = -1;
                product[i].wait_time = -1;
            }
            pthread_mutex_unlock(&lock);
            //printf("Mutex released by Consumer\n");
            sleep(time);

        } 
        //else {
        //    printf("Consumer did not obtain Mutex.\n");
        //}
        i++;
    }
}


int main()
{
    int i;
    int producer_wait;
    for (i = 0; i < 32; ++i){
        product[i].print=-1;
        product[i].wait_time=-1;
    }

    //Prints all the data in the array.
    //for (i = 0; i < 32; ++i){
    //    printf("Print: %d\n", product[i].print);
    //    printf("Wait: %d\n", product[i].wait_time);
    //}

    /* this variable is our reference to the second thread */
    pthread_t consumer_thread;

    /* create a second thread which executes inc_x(&x) */
    if(pthread_create(&consumer_thread, NULL, (void *)consumer, (void *)NULL)) {

        fprintf(stderr, "Error creating thread\n");
        return 1;

    }
    while (1){

        if (i >= 32 ) {
            i = 0;
        }
        
        producer_wait = 0;
        
        if (!check_mutex()){
            //printf("Mutex Aquired by Producer\n");
            if (check_empty(i)) {
                product[i].print = rdrand_func(3);
                product[i].wait_time = rdrand_func(1);
                printf("Producer Inserting Value %d and Wait Time %d at Index: %d\n", product[i].print, product[i].wait_time, i);
                producer_wait = rdrand_func(2);
                printf("Producer is now sleeping for %d seconds\n", producer_wait);
            }
            pthread_mutex_unlock(&lock);
            //printf("Mutex released by Producer\n");
            sleep(producer_wait);
        } 
        
        //else {
          //  printf("Producer did not obtain Mutex.\n");
        //}
        i++;
    }
    /* wait for the second thread to finish */
    if(pthread_join(consumer_thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    return 0;

}
