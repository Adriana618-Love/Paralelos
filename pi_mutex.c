/*Compilar:  gcc -g -Wall -o pi_mutex pi_mutex.c -lm -lpthread
 *Ejecutar:      ./pi_mutex <number of threads> <n>
 *           n debe ser divisible entre el número de threads
 */        

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

const int MAX_THREADS = 1024;

long thread_count;
long long n;
double sum;

#include <sys/time.h>

/* RE100 robado de internet */
#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}

pthread_mutex_t mutex;

void* Thread_sum(void* rank);

/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial_pi(long long n);

int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double start, finish, elapsed;

   /* Get number of threads from command line */
   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   pthread_mutex_init(&mutex, NULL);
   sum = 0.0;

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Thread_sum, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   GET_TIME(finish);
   elapsed = finish - start;

   sum = 4.0*sum;
   printf("Con n = %lld terminos,\n", n);
   printf("Varios thread con busy-wait  = %.15f\n", sum);
   printf("Tiempo Transcurrido = %e seconds\n", elapsed);
   GET_TIME(start);
   sum = Serial_pi(n);
   GET_TIME(finish);
   elapsed = finish - start;
   printf("Calculo secuencial = %.15f\n", sum);
   printf("Tiempo Transcurrido = %e seconds\n", elapsed);
   printf("Valor original de Pi    = %.15f\n", 4.0*atan(1.0));
   
   pthread_mutex_destroy(&mutex);
   free(thread_handles);
   return 0;
}  /* main */


void* Thread_sum(void* rank) { /*RE100 robado del libro*/
   long my_rank = (long) rank;
   double factor;
   long long i;
   long long my_n = n/thread_count;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;
   double my_sum = 0.0;

   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;

   for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      my_sum += factor/(2*i+1);
   }
   pthread_mutex_lock(&mutex);
   sum += my_sum;
   pthread_mutex_unlock(&mutex);

   return NULL;
}  /* Thread_sum */

double Serial_pi(long long n) {
   double sum = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }
   return 4.0*sum;

}  /* Serial_pi */

void Get_args(int argc, char* argv[]) {
   thread_count = strtol(argv[1], NULL, 10);  
   n = strtoll(argv[2], NULL, 10);
}  /* Get_args */