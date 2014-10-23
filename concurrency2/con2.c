/******************************************************************************
 * Concurrency Project #2
 * CS444
 * Authors: Sam Quinn, Yi Li, Lawrence Chau
 * 10/22/2014
 ******************************************************************************/

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *dinner_order(void *ph);
void dinner_start();

typedef struct philoso_struct 
{
  int status;
  const char *name;
  pthread_mutex_t *fork_left, *fork_right;
  pthread_t thread;
    
}Philos;

int main()
{
  dinner_start();
  return 0;
} 
 
void dinner_start()
{ 
  
  int i;
  int fail_attemp;

  const char *names[] = { "Sam", "Yi", "Lawrence", "Kevin", "Flash" };
    
  Philos *phil;
  pthread_mutex_t forks[5];
  Philos philosophers[5];
   
  for (i = 0; i < 5; i++) 
    {
      fail_attemp = pthread_mutex_init(&forks[i], NULL);
      if (fail_attemp) 
	{
	  printf("Error: fail to initialize mutexe.");
	  exit(1);
        }
    }
 
  for (i = 0; i < 5; i++) 
    {
      phil = &philosophers[i];
      phil->name = names[i];
      phil->fork_left = &forks[i];
      phil->fork_right = &forks[(i + 1) % 5];

      phil->status = pthread_create( &phil->thread, NULL, dinner_order, phil);
    }
 
  for(i = 0; i < 5; i++) 
    {
      phil = &philosophers[i];
      if ( !phil->status && pthread_join( phil->thread, NULL) ) 
	{
	  printf("Error: fail to join thread %s", phil->name);
	  exit(1);
        }
    }
}
 
void *dinner_order(void *ph) 
{
  int fail_attemp;
  int attemp = 2;

  Philos *phil = (Philos*)ph;
  pthread_mutex_t *fork_left, *fork_right;
  pthread_mutex_t *temp;
 
  while (1) 
    {
      printf("%s is thinking\n", phil->name);
      sleep( 1 + rand()%20);
 
      fork_left = phil->fork_left;
      fork_right = phil->fork_right;
      printf("%s get fork\n", phil->name);

      while(fail_attemp) 
	{
	  fail_attemp = pthread_mutex_lock( fork_left);
	  fail_attemp = (attemp > 0) ? pthread_mutex_trylock(fork_right) : pthread_mutex_lock(fork_right);

	  if (fail_attemp) 
	    {
	      pthread_mutex_unlock( fork_left);
	      temp = fork_left;
	      fork_left = fork_right;
	      fork_right = temp;
	      attemp -= 1;
            }
        }
 
      if (!fail_attemp) 
	{
	  printf("%s is eating\n", phil->name);
	  sleep( 2 + rand() % 9);

	  pthread_mutex_unlock( fork_right);
	  pthread_mutex_unlock( fork_left);
	  //sleep( 1 + rand() % 8);
	  printf("%s put fork\n", phil->name);
        }
    }

  return NULL;
}/* Concurency Problem #2 */

