
// Productor Consumidor solucionado con mutex y variables condición
// Asignatura: Sistemas Distribuidos y Concurrentes
// Universidad Rey Juan Carlos

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_BUFFER 10
#define MAX_DATA_PRODUCER 50
#define SLEEP_TIME 50000

#define MAX_PRODUCERS 10

pthread_mutex_t mutex;
pthread_cond_t not_full;
pthread_cond_t not_empty;

int n_elems=0;
int buffer[MAX_BUFFER];
int pos = 0 ;

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

    pthread_mutex_lock(&mutex);
    int producer_id = *(int *) args;
    blue();
    printf("[PRODUCER ID=%d][%d] \t ENTER in critial region\n", producer_id,i); 
    reset();

    while (n_elems == MAX_BUFFER) {
      red();
      printf("[PRODUCER ID=%d][%d] \t WAITING in condition\n", producer_id,i);
      reset();
      
      pthread_cond_wait(&not_full, &mutex);      
    }
    yellow();
    printf("[PRODUCER ID=%d][%d] \t AFTER condition\n", producer_id,i);
    reset();

    buffer[pos] = i;
    aux_pos = pos;
    pos = (pos + 1) % MAX_BUFFER;
    n_elems ++;

    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);
    
    purple();
    printf("[PRODUCER ID=%d][%d] \t BUFFER[%d]=%d \n", producer_id, i, aux_pos, i);
    reset();

    usleep(SLEEP_TIME);
  }
  pthread_exit(0);
}

void consumer (void) {

  int data = 0;
  int pos = 0;
  int aux_pos = 0;

  for (int i=0; i<MAX_DATA_PRODUCER*MAX_PRODUCERS; i++) {

    pthread_mutex_lock(&mutex);

    while (n_elems == 0) {
      purple();
      printf("[CONSUMER] %d \t WAITING in condition\n", i);      
      reset();      
      pthread_cond_wait(&not_empty, &mutex);      

    }
      

    data = buffer[pos];
    aux_pos = pos;
    pos = (pos + 1) % MAX_BUFFER;
    n_elems--;

    // Prueba a ver que pasa si pthread_cond_signal lo modificas por un pthread_cond_broadcast
    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mutex);

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

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&not_full, NULL);
  pthread_cond_init(&not_empty, NULL);


  pthread_create(&th_consumer, NULL, (void *)&consumer, NULL );

  for (int i=0; i<MAX_PRODUCERS; i++) {
    ids[i] = i;    
    pthread_create(&th_producer[i], NULL, (void *)&producer, (void *) &ids[i]);
    
  }
    
  pthread_join(th_consumer, NULL);

  for (int i=0; i<MAX_PRODUCERS; i++) {
    pthread_join(th_producer[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&not_full);
  pthread_cond_destroy(&not_empty);

  return 0;

}
