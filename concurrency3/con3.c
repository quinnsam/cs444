#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define RAND (rand()%10+1)

pthread_mutex_t ser_lock;
pthread_mutex_t del_lock;
pthread_mutex_t ins_lock;

struct linkList {
    int data;
    struct linkList *next;
}*head;


int count(){
    struct linkList *temp;
    int i=0;
    temp = head;
    while (temp != NULL){
        temp = temp->next;
        i++;
    }
    return i;
}


void searcher () {
    //printf("IN Searcher\n");
    struct linkList *temp;
    int del;
    while (1) {
        del = 1;
        //printf("S");
        if (!pthread_mutex_trylock(&ser_lock)){
            //if (!pthread_mutex_trylock(&del_lock)){
            temp = head;
            if (temp == NULL){
                printf("List is empty\n");
                continue;
            } else {

                while (temp != NULL){
                    printf("[%d] ", temp->data);
                    temp = temp->next;
                }
                printf("\n");
            }
            pthread_mutex_unlock(&ser_lock);
        }
        //pthread_mutex_unlock(&del_lock);
        sleep(1);
        //}
    }
}

void inserter () {
    struct linkList *temp, **tail;
    int insertValue, i, del;
    while (1){
        del = 1;
        if (count() < 20){
            //printf("I");
            if (!pthread_mutex_lock(&ins_lock)){
                //if(!pthread_mutex_trylock(&del_lock)){

                temp = (struct linkList *)malloc(sizeof(struct linkList));
                insertValue = RAND;

                printf("inserting--%d\n", insertValue);
                temp->data = insertValue;
                temp->next = NULL;
                tail = &head;
                if (head == NULL){
                    head = temp;
                } else {
                    while (*tail != NULL){
                        tail = &((*tail)->next);
                    }
                    *tail = temp;
                }
                //}

                pthread_mutex_unlock(&ins_lock);
                //pthread_mutex_unlock(&del_lock);
                sleep(5);
            }
        }
    }
}


void deleter () {
    //printf("IN Deleter\n");
    int deleteValue;
    struct linkList *temp, *prev;
    while (1){
        //printf("D");
        if (count() > 1){
            //printf("I:%d, D:%d, S:%d\n", pthread_mutex_trylock(&ins_lock), pthread_mutex_trylock(&del_lock), pthread_mutex_trylock(&ser_lock));
            if (!pthread_mutex_trylock(&ins_lock)){
                if(!pthread_mutex_trylock(&ser_lock)){

                    //if (!pthread_mutex_trylock(&lock)){
                    temp = head;
                    deleteValue = RAND;
                    while (temp != NULL) {
                        if(temp->data == deleteValue){
                            printf("Deleting: %d\n", deleteValue);
                            if (temp == head) {
                                head = temp->next;
                                free(temp);
                                break;
                            } else {
                                prev->next = temp->next;
                                free(temp);
                                break;
                            }
                        } else {
                            prev = temp;
                            temp = temp->next;
                        }
                    }
                    pthread_mutex_unlock(&ser_lock);
                }
                    pthread_mutex_unlock(&ins_lock);
                }
                sleep(2);
            }
        }
    }

    int main(){
        int i = 0;


        struct linkList *new;
        new = (struct linkList *)malloc(sizeof(struct linkList));
        new->data = RAND;
        head = new;
        head->next = NULL;

        //for(i=0; i<23;++i){
        //    inserter();
        //}

        //searcher();

        //for(i=0; i<12;++i){
        //    deleter();
        //}
        //searcher();


        /* this variable is our reference to the second thread */
        pthread_t search_thread[3];
        pthread_t insert_thread[3];
        pthread_t delete_thread[3];

        for(i = 0; i < 3; ++i){
            /* create a second thread which executes inc_x(&x) */
            //printf("Creating insetrter\n");
            if(pthread_create(&insert_thread[i], NULL, (void *)inserter, (void *)NULL)) {
                fprintf(stderr, "Error creating inserter thread\n");
                return 1;
            }

            //printf("Creating searcher\n");
            if(pthread_create(&search_thread[i], NULL, (void *)searcher, (void *)NULL)) {
                fprintf(stderr, "Error creating searcher thread\n");
                return 1;
            }

            //printf("Creating deleter\n");
            if(pthread_create(&delete_thread[i], NULL, (void *)deleter, (void *)NULL)) {
                fprintf(stderr, "Error creating delete thread\n");
                return 1;
            }
        }
        for(i = 0; i < 3; ++i){
            pthread_join(search_thread[i], NULL);
            pthread_join(insert_thread[i], NULL);
            pthread_join(delete_thread[i], NULL);
        }
        return 0;
    }
