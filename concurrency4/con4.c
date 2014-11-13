#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define seats 6

void *customerMaker();
void *barberShop();
void *waitingRoom();
void checkQueue();

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sleep_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barberSleep_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t barberWorking_cond = PTHREAD_COND_INITIALIZER;

int returnTime=5,current=0, sleeping=0, iseed;

int main(int argc, char *argv[])
{
		iseed=time(NULL);
		srand(iseed);
		//declare barber thread;
		pthread_t barber,customerM,timer_thread;
		pthread_attr_t barberAttr, timerAttr;
		pthread_attr_t customerMAttr;

		//define barber, and cutomerMaker default attributes
		pthread_attr_init(&timerAttr);
		pthread_attr_init(&barberAttr);
		pthread_attr_init(&customerMAttr);

		printf("\n");

		//create cutomerMaker
		pthread_create(&customerM,&customerMAttr,customerMaker,NULL);

		//create barber
		pthread_create(&barber,&barberAttr,barberShop,NULL);

		pthread_join(barber,NULL);
		pthread_join(customerM,NULL);

		return 0;
}

void *customerMaker()
{
		int i=0;
		printf("*Customer Maker Created*\n\n");
		fflush(stdout);
		pthread_t customer[seats+1];
		pthread_attr_t customerAttr[seats+1];
		while(i<(seats+1))
		{
				i++;
				pthread_attr_init(&customerAttr[i]);
				while(rand()%2!=1)
				{
						sleep(1);
				}
				pthread_create(&customer[i],&customerAttr[i],waitingRoom,NULL);
		}
		pthread_exit(0);
}

void *waitingRoom()
{
		//take seat
		pthread_mutex_lock(&queue_mutex);
		checkQueue();

		sleep(returnTime);
		waitingRoom();
}

void *barberShop()
{
		int loop=0;
		printf("The barber has opened the store.\n");
		fflush(stdout);
		while(loop==0)
		{
				if(current==0)
				{
						printf("\tThe shop is empty, barber is sleeping.\n");
						fflush(stdout);
						pthread_mutex_lock(&sleep_mutex);
						sleeping=1;
						pthread_cond_wait(&barberSleep_cond,&sleep_mutex);
						sleeping=0;
						pthread_mutex_unlock(&sleep_mutex);
						printf("\t\t\t\tBarber wakes up.\n");
						fflush(stdout);
				}
				else
				{
						printf("\t\t\tBarber begins cutting hair.\n");
						fflush(stdout);
						sleep((rand()%20)/5);
						current--;
						printf("\t\t\t\tHair cut complete, customer leaving store.\n");
						pthread_cond_signal(&barberWorking_cond);
				}
		}
		pthread_exit(0);
}

void checkQueue()
{
		current++;
		printf("\tCustomer has arrived in the waiting room.\t\t\t\t\t\t\t%d Customers in store.\n",current);
		fflush(stdout);
		printf("\t\tCustomer checking chairs.\n");
		fflush(stdout);
		if(current<seats)
		{
				if(sleeping==1)
				{
						printf("\t\t\tBarber is sleeping, customer wakes him.\n");
						fflush(stdout);
						pthread_cond_signal(&barberSleep_cond);
				}
				printf("\t\tCustomer takes a seat.\n");
				fflush(stdout);
				pthread_mutex_unlock(&queue_mutex);
				pthread_mutex_lock(&wait_mutex);
				pthread_cond_wait(&barberWorking_cond,&wait_mutex);
				pthread_mutex_unlock(&wait_mutex);
				return;
		}
		if(current>=seats)
		{
				printf("\t\tAll chairs full, leaving store.\n");
				fflush(stdout);
				current--;
				pthread_mutex_unlock(&queue_mutex);
				return;
		}
}

