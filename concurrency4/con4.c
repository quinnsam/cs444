#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
<<<<<<< HEAD
#include <time.h>
#define MAX_SEATS 10
#define WAIT_TIME 10

//Mutex for accessing the shop
pthread_mutex_t shop_mutex;
//pthread_mutex_init(&shop_mutex, NULL);

//Mutex for cutting hair next
pthread_mutex_t nextCustomer_mutex;
//pthread_mutex_init(&nextCustomer_mutex, NULL);

//Mutex for when the barber needs a nap
pthread_mutex_t barberSleep_mutex;
//pthread_mutex_init(&barberSleep_mutex, NULL);

//Whether the barber is sleeping
pthread_cond_t barberSleepStatus_cond;
//pthread_cond_init(&barberSleepStatus_cond, NULL);

//Whether the barber is working
pthread_cond_t barberWorkStatus_cond;
//pthread_cond_init(&barberWorkStatus_cond, NULL);

int numCustomers = 0;
int barberSleeping = 0;
int haircutTime = 0;

void get_hair_cut();
void cut_hair();
void *generate_customer();
void customer();
void *waiting_room();
void *barber_room();


void get_hair_cut(){
    printf("Customer is getting a haircut... \n");
    sleep(haircutTime);
 }

void cut_hair(){
    printf("Barber is cutting hair... \n");
    sleep(haircutTime);
}
//This thread creates threads for each customer, up to the max number of seats defined plus one. The moment whether or not a customer walks in (AKA generated) is based on a random number. If the the number is even, a customer is generated. If odd, then it sleeps and attemps to generate a customer again once it wakes up.
void *generate_customer(){
    
    int i = 0;
    pthread_t customers[MAX_SEATS + 1];
    pthread_attr_t customers_attr[MAX_SEATS + 1];
   

    //[SEG FAULT] If you run this program, a seg fault occurs once the maximum number of threads have been created and the function tries to create more. I believe the problem is that I am not destroying the thread/customer after they are finished with the haircut or when they leave the store. If someone can help me with that, that would be cool. 
    while(1){
        
        sleep(rand()%6+1);

        for(i=0; i < (MAX_SEATS + 1); ++i) {
                pthread_attr_init(&customers_attr[i]);
                pthread_create(&customers[i], &customers_attr[i], waiting_room, NULL);
        }
    }
}

void customer(){
    
    //First checks to see if the there is an available seat.
    if(numCustomers < MAX_SEATS){

        //Then checks to see if the barber is asleep. If so, wake him up.
        if(barberSleeping == 1){
            printf("The barber is sleeping! The customer pokes the barber to wake him up. \n");
            pthread_cond_signal(&barberSleepStatus_cond);
        }

        //The customer takes the mutex and awaits for the barber's signal to sit in barber seat.
        pthread_mutex_unlock(&shop_mutex);
        printf("There is a seat! The customer sits down.\n");
        pthread_mutex_lock(&nextCustomer_mutex);
        pthread_cond_wait(&barberWorkStatus_cond, &nextCustomer_mutex);
        get_hair_cut();
        pthread_mutex_unlock(&nextCustomer_mutex);
        pthread_exit(0);
        numCustomers--;
    }
    
    //If there are no available seats, the customer leaves.
    if(numCustomers >= MAX_SEATS){
        numCustomers--;
        pthread_mutex_unlock(&shop_mutex);
        printf("All the seats are filled :( The customer sadly leaves the shop... \n");
        //return;
        pthread_exit(0);
    }
}

//The customer enters the shop and attempts to take a seat
void *waiting_room(){ 
    
   //while(1){ 
        pthread_mutex_lock(&shop_mutex); 
        numCustomers++; 
        printf("A customer has entered the waiting room. Checking for available seats...\n");
        customer();
   //}

}

void *barber_room(){

    while(1){    
        //If there are no customers, the barber will fall asleep until a customer wakes him up.
        if(numCustomers == 0){
            pthread_mutex_lock(&barberSleep_mutex);
            barberSleeping = 1;
            printf("The lack of customer is making the barber sleepy. He decides to take a quick snooze.\n");
            pthread_cond_wait(&barberSleepStatus_cond, &barberSleep_mutex);
            barberSleeping = 0;
            printf("The barber woke up! Time to get back to work.\n");
        }
        //The barber will signal for the next customer to begin the haircut.
        else{
            printf("The barber calls for the next customer.\n");
            srand(time(NULL));
            haircutTime = rand()%10;
            pthread_cond_signal(&barberWorkStatus_cond);
            cut_hair();
            printf("The barber finished cutting the hair!\n");
        }      
    }   
}


int main(int argc, char *argv[]){

    pthread_t barber;
    pthread_t customersGenerator;
    pthread_t timer;
    
    pthread_attr_t barber_attr;
    pthread_attr_t customersGenerator_attr;
    pthread_attr_t timer_attr;
    
    pthread_attr_init(&barber_attr);
    pthread_attr_init(&customersGenerator_attr);
    pthread_attr_init(&timer_attr);

    pthread_create(&barber, &barber_attr, barber_room, NULL);
    pthread_create(&customersGenerator, &customersGenerator_attr, generate_customer, NULL);

    pthread_join(barber, NULL);
    pthread_join(customersGenerator, NULL);

    return 0;
}

=======
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

>>>>>>> f230fb5a3613e7fdf7b5bdd86c9b59c0c2aefe06
