/* File:       
 *    mpi_hello.c
 *
 * Purpose:    
 *    A "hello,world" program that uses MPI
 *
 * Compile:    
 *    mpicc -g -Wall -std=c99 -o mpi_hello mpi_hello.c
 * Usage:        
 *    mpiexec -n 2 ./mpi_hello
 *
 * Input:      
 *    None
 * Output:     
 *    A funny game of pinpon between messages
 *
 * Algorithm:  
 *    Each process sends a message to process 0, which prints 
 *    the messages it has received, as well as its own message.
 *
 * IPP:  Section 3.1 (pp. 84 and ff.)
 */
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */ 

const int LIMIT = 10;

int main(void) {
   int *ball;
   ball = malloc(sizeof(int));
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */

   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank != 0) { 
      while(1){
         /*Send message*/
         (*ball)++;
         MPI_Send(ball, 1, MPI_INT, 0, 6, MPI_COMM_WORLD);
         if(*ball>LIMIT)break;
         /*Receive message*/
         MPI_Recv(ball, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         printf("Receive from process %d of %d with ball = %d\n", my_rank, comm_sz,*ball);
      }
      printf("Finalizado process %d\n",my_rank);
   } else {  
      while(1){
         /*Receive message*/
         MPI_Recv(ball, 1, MPI_INT, 1, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         printf("Receive from process %d of %d! with ball = %d\n", my_rank, comm_sz,*ball);
         (*ball)++;
         if(*ball>LIMIT)break;
         /*Send message*/
         MPI_Send(ball, 1, MPI_INT, 1, 6, MPI_COMM_WORLD);
      }
      printf("Finalizado process %d\n",my_rank);
   }

   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */