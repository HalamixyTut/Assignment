#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#define MAX_NUM 10

void *add_array (void *arg);



int thread_count;
long no_elts; 
long range;
long *array;
long *thread_index;

void generate_numbers()
{
	int i;

	srandom(time(NULL));

	for(i=0; i<no_elts; i++)
		array[i] = random()%MAX_NUM;
}



void print_numbers() 
{
	int i;

	for(i=0; i<no_elts; i++) 
		printf("%ld ", array[i]);
	printf("\n");
}





int main (int argc, char **argv)
{
	char *str_thread_count;
	int r, trunc; 
	pthread_t *tid;
        int sfd;
	long total_sum = 0;

	thread_count = atoi(argv[1]);
	no_elts = atoi(argv[2]);

	range = no_elts / thread_count;	


	thread_index = (long *) malloc(sizeof(long)*thread_count);
	array = (long *) malloc(sizeof(long)*no_elts);
	tid = (pthread_t*) malloc(sizeof(pthread_t)*thread_count);


	if(tid == NULL)
		perror ("Cannot allocate memory for threads\n");

	generate_numbers();
	print_numbers();
	
       
	for (int i = 0; i < thread_count; i++) {
		thread_index[i] = i;
	
		if ((r = pthread_create (&tid [i], NULL, add_array, (void*)&thread_index[i])) != 0) {

			perror("Cannot create multiple threads\n");
		}
		
	}


	// Wait for threads to terminate
	for (int i = 0; i < thread_count; i++) {
		if ((r = pthread_join (tid [i], NULL)) != 0) {

			perror("Cannot join multiple threads\n");
		}
	}

	for (int i=0 ; i<thread_count; i++)
		total_sum = total_sum + thread_index[i];	
       
	printf("Total Sum: %ld\n", total_sum);
         

	
	exit (0);
}


void *add_array(void *args){
	int id = *(int *)args;
	long start = id*range;
	long end = start + range;
	long local_sum = 0;


	for(long i = start; i < end; i++){
		local_sum = array[i]+local_sum;
	}

	printf("Start:%ld End:%ld Sum:%ld\n", start, end, local_sum);
	thread_index[id] = local_sum;
	return NULL;
	
}