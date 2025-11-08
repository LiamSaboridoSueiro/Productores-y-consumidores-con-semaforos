#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_BUFFER 10
#define MAX_DATA_PRODUCER 50
#define SLEEP_TIME 50000

#define MAX_PRODUCERS 10

sem_t mutex;
sem_t espacios_vacios;
sem_t espacios_ocupados;

int n_elems=0;
int buffer[MAX_BUFFER];
int pos = 0;

void reset () {
  printf("\033[0m");
}

void blue () {
  printf("\033[0;34m");
}

void red () {
  printf("\033[1;31m");
}

void yellow () {
  printf("\033[1;33m");
}

void purple () {
  printf("\033[0;35m");
}


void* producer (void* args ) {
  
  int aux_pos = 0;

  for (int i=0; i<MAX_DATA_PRODUCER; i++) {

    int producer_id = *(int *) args;

    red();
    printf("[PRODUCER ID=%d][%d] \t WAITING for semaphore\n", producer_id,i);
    reset();

    sem_wait(&espacios_vacios);

    yellow();
    printf("[PRODUCER ID=%d][%d] \t AFTER semaphore\n", producer_id,i);
    reset();

    sem_wait(&mutex);

    blue();
    printf("[PRODUCER ID=%d][%d] \t ENTER in critical region\n", producer_id, i); 
    reset();
    

    buffer[pos] = i;
    aux_pos = pos;
    pos = (pos + 1) % MAX_BUFFER;
    n_elems++;

    sem_post(&mutex);
    sem_post(&espacios_ocupados);
    
    purple();
    printf("[PRODUCER ID=%d][%d] \t BUFFER[%d]=%d \n", producer_id, i, aux_pos, i);
    reset();

    usleep(SLEEP_TIME);
  }
  pthread_exit(0);
}

void consumer (void) {

  int data = 0;
  int pos_consumer = 0;
  int aux_pos = 0;

  for (int i=0; i<MAX_DATA_PRODUCER*MAX_PRODUCERS; i++) {

    purple();
    printf("[CONSUMER] %d \t WAITING for semaphore\n", i);      
    reset();
    
    sem_wait(&espacios_ocupados);
    sem_wait(&mutex);

    data = buffer[pos_consumer];
    aux_pos = pos_consumer;
    pos_consumer = (pos_consumer + 1) % MAX_BUFFER;
    n_elems--;

    sem_post(&mutex);
    sem_post(&espacios_vacios);

    purple();
    printf("CONSUMER \t\t BUFFER[%d]=%d \n", aux_pos, data);
    reset();
    
    usleep(SLEEP_TIME*3);
  }
  pthread_exit(0);
}


int main(void)
{
  pthread_t th_consumer, th_producer[MAX_PRODUCERS];
  int ids[MAX_PRODUCERS];

  sem_init(&mutex, 0, 1);
  sem_init(&espacios_vacios, 0, MAX_BUFFER);
  sem_init(&espacios_ocupados, 0, 0);

  pthread_create(&th_consumer, NULL, (void *)&consumer, NULL);

  for (int i=0; i<MAX_PRODUCERS; i++) {
    ids[i] = i;    
    pthread_create(&th_producer[i], NULL, (void *)&producer, (void *) &ids[i]);
  }
    
  pthread_join(th_consumer, NULL);

  for (int i=0; i<MAX_PRODUCERS; i++) {
    pthread_join(th_producer[i], NULL);
  }

  sem_destroy(&mutex);
  sem_destroy(&espacios_vacios);
  sem_destroy(&espacios_ocupados);

  return 0;
}