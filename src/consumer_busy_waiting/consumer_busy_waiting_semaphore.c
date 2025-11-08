
// Productor Consumidor solucionado con mutex y variables condición
// Asignatura: Sistemas Distribuidos y Concurrentes
// Universidad Rey Juan Carlos

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define MAX_BUFFER 10
#define MAX_DATA_PRODUCER 50
#define SLEEP_TIME 50000


sem_t mutex; // mutex de zona crítica, no tengo seguro si haría falta ya que se protege con 2 semaforos los buffers (ponerlo no hace daño supongo)
sem_t espacios_vacios;
sem_t espacios_ocupados;


int buffer[MAX_BUFFER];


void producer (void) {
    int pos = 0;
    int aux_pos = 0;
    
    for (int i=0; i<MAX_DATA_PRODUCER; i++) {
        sem_wait(&espacios_vacios);

        sem_wait(&mutex);
        buffer[pos] = i;
        aux_pos = pos;
        pos = (pos + 1) % MAX_BUFFER;
        sem_post(&mutex);

        sem_post(&espacios_ocupados);
        
        printf("Producer[%d] %d \n", aux_pos, i);
        usleep(SLEEP_TIME);
    }
    pthread_exit(0);
}

void consumer (void) {
    int data = 0;
    int pos = 0;
    int aux_pos = 0;
    
    for (int i=0; i<MAX_DATA_PRODUCER; i++) {
        sem_wait(&espacios_ocupados);

        sem_wait(&mutex);
        data = buffer[pos];
        aux_pos = pos;
        pos = (pos + 1) % MAX_BUFFER;
        sem_post(&mutex);

        sem_post(&espacios_vacios);
        
        printf("Consumer[%d] %d \n", aux_pos, data);
        usleep(SLEEP_TIME*3);
    }
    pthread_exit(0);
}

int main(void) {
    pthread_t th_consumer, th_producer;
    
    sem_init(&mutex, 0, 1);
    sem_init(&espacios_vacios, 0, MAX_BUFFER);
    sem_init(&espacios_ocupados, 0, 0);
    
    pthread_create(&th_producer, NULL, (void *)&producer, NULL);  
    pthread_create(&th_consumer, NULL, (void *)&consumer, NULL);
    
    pthread_join(th_producer, NULL);
    pthread_join(th_consumer, NULL);
    
    sem_destroy(&mutex);
    sem_destroy(&espacios_vacios);
    sem_destroy(&espacios_ocupados);
    
    return 0;
}