#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUMBER_OF_POINTS   50000000
#define NUMBER_OF_THREADS 4

int sphere_count = 0;
pthread_mutex_t mutex;
void *runner(void *param);

int main (int argc, const char * argv[]) {
  int points_per_thread = NUMBER_OF_POINTS / NUMBER_OF_THREADS;
  double est_pi;

  pthread_t runners[NUMBER_OF_THREADS];
  pthread_mutex_init(&mutex, NULL);

  int i;
  clock_t start, end;
  double cpu_time_used;

  start = clock();
  for (i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_create(&runners[i], 0, runner, &points_per_thread);
  }
  
  for (i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(runners[i], NULL);
  }

  end = clock();

  est_pi = pow(2.0, 3.0) * sphere_count / NUMBER_OF_POINTS;
  printf("Estimated Volume from %d points = %f\n", NUMBER_OF_POINTS, est_pi);
  printf("Actual Volume with the formula (4*pi*r^3)/3: %f\n", (4*3.1415926*pow(1, 3)/3));
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Time used: %f\n", cpu_time_used);
  return 0;
}


void *runner(void *param) { 
  int points = *((int *)param);
  int hits = 0;
  double x, y, z, f; 

  int i;

  for (i = 0; i < points; i++) {
    x = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    y = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    z = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;

    x = pow(x, 2.0);
    y = pow(y, 2.0);
    z = pow(z, 2.0);

    f = sqrt(x + y + z);

    if (f <= 1.0 && f >= -1) {
      hits++;
    }
  }

    printf("hits = %d\n", hits);

    pthread_mutex_lock(&mutex);
    sphere_count = sphere_count + hits; 
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}