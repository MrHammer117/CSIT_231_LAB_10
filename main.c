#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUMBER_OF_DARTS 50
#define NUMBER_OF_THREADS 4

int circle_count = 0;
pthread_mutex_t mutex;
void *runner(void *param);

int main (int argc, const char * argv[]) {
  int points_per_thread = NUMBER_OF_DARTS / NUMBER_OF_THREADS;
  double est_pi;

  pthread_t runners[NUMBER_OF_THREADS];
  pthread_mutex_init(&mutex, NULL);

  int i;

  for (i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_create(&runners[i], 0, runner, &points_per_thread);
  }

  for (i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(runners[i], NULL);
  }

  est_pi = 4.0 * circle_count / NUMBER_OF_DARTS;
  printf("Estimated Pi from %d darts = %f\n", NUMBER_OF_DARTS, est_pi);

  return 0;
}


void *runner(void *param) { 
  int points = *((int *)param);
  int hits = 0;
  double x, y, z; 

  int i;

  for (i = 0; i < points; i++) {
    x = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    y = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;

    x = pow(x, 2.0);
    y = pow(y, 2.0);

    z = sqrt(x + y);

    if (z < 1.0) {
      hits++;
    }
  }

    printf("hits = %d\n", hits);

    pthread_mutex_lock(&mutex);
    circle_count = circle_count + hits; 
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}