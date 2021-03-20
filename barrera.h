#include <pthread.h>

struct cond_barrier{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    unsigned int thread_target;
    unsigned int thread_counter;
    int cycle;
};

typedef struct cond_barrier barrier_t;

/* Creación de una barrera de condición, tomando como argumento la cantidad de
hilos que se van a esperar*/
void barrier_init(barrier_t *barr, unsigned int count) {
  barr->thread_target = barr->thread_counter = count;
  barr->cycle = 0;
  pthread_mutex_init(&barr->mutex, NULL);

  //inicializa la variable de condicion
  pthread_cond_init(&barr->cond, NULL);
}

/* Función *bloqueante* para esperar a los demás hilos */
void barrier_wait(barrier_t *barr) {
  int status, cycle;
  pthread_mutex_lock(&barr->mutex);
  cycle = barr->cycle;


  barr->thread_counter--;

  if (barr->thread_counter){
    while (cycle == barr->cycle) {
      status = pthread_cond_wait(&barr->cond, &barr->mutex);
      if (status != 0)
        break;
    }
  }

  // ultimo thread que trabaja
  else{
    barr->cycle = !barr->cycle;
    barr->thread_counter = barr->thread_target;
    pthread_cond_broadcast(&barr->cond);
  }

    pthread_mutex_unlock(&barr->mutex);
}


/* Eliminación de la barrera */
void barrier_destroy(barrier_t *barr) {
  pthread_mutex_destroy(&barr->mutex);
  pthread_cond_destroy(&barr->cond);
}
