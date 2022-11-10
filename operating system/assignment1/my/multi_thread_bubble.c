/* Even-Odd Sequential Bubble Sort
 * Author: Kartik Gopalan
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <wait.h>
#include <string.h>
#include <pthread.h>

#define MAX_COUNT 1000000 // Max integers to sort
#define MAX_NUM 100		  // Generate numbers between 0 and MAX_NUM

// Uncomment the following line to turn on debugging messages
//#define DEBUG

long number[MAX_COUNT];
int N; // Number of integers to sort
int P; // Number of threads

key_t key;
size_t size;

int shmid;
int pid;

long *shm;
int swap_count;

// generate N random numbers between 0 and MAX_NUM
void generate_numbers()
{
	int i;

	srandom(time(NULL));

	for (i = 0; i < N; i++)
		number[i] = random() % MAX_NUM;
}

void print_numbers()
{
	int i;

	for (i = 0; i < N; i++)
		printf("%ld ", number[i]);
	printf("\n");
}

int compare_and_swap(int i, int j)
{
#ifdef DEBUG
	fprintf(stderr, "i %d j %d\n", i, j);
#endif
	assert(i < N);
	assert(j < N);

	if (number[i] > number[j])
	{
		long temp = number[i];
		number[i] = number[j];
		number[j] = temp;
		return 1;
	}

	return 0;
}

void* printftest(){
	printf("test\n");
}

// even-odd pass bubbling from start to start+n
void bubble(int start, int n, int pass)
{
#ifdef DEBUG
	fprintf(stderr, "start %d n %d pass %d\n", start, n, pass);
#endif

	int next = start;

	assert(start < N - 1); // bug if we start at the end of array

	while ((next + 1) < (start + n))
	{
		swap_count += compare_and_swap(next, next + 1);
		next += 2;
	}
}

void bubble_sort()
{
	int start = 0;
	int n;
	int pass = 0;
	int number_count;
	int last_count = 1;
	int tempswap = 0;

	while (last_count != 0 || tempswap != 0)
	{
		if (pass == 0)
		{
			number_count = N;
			start = 0;
		}
		else
		{
			number_count = N - 1;
			start = 1;
		}
		
		tempswap = last_count;
		last_count = 0;

		for (int i = 0; i < P; i++)
		{
			n = ((number_count / 2) / P);
			if (((number_count / 2) % P) > i)
				n += 1;
			n *= 2;

			if (n != 0){
				bubble(start, n, 0);
				last_count += swap_count;
				swap_count = 0;
			}
			start += n;	
		}
		pass = pass == 0 ? 1 : 0;
	}
}

void* bubble_sort1(void *args){
	bubble_sort();
}

int main(int argc, char *argv[])
{
	struct timeval starttime;
	struct timeval endtime;
	gettimeofday(&starttime, NULL);
	int n, r, number_count;
	int pass = 0;
	int start = 0;
	pthread_t *tid;
	
	tid = (pthread_t*) malloc(sizeof(pthread_t)*P);

	if(tid == NULL)
		perror ("Cannot allocate memory for threads\n");

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s N\n", argv[0]);
		return 1;
	}
	
	N = strtol(argv[1], (char **)NULL, 10);
	if ((N < 2) || (N > MAX_COUNT))
	{
		fprintf(stderr, "Invalid N. N should be between 2 and %d.\n", MAX_COUNT);
		return 2;
	}

	P = strtol(argv[2], (char **)NULL, 10);

	generate_numbers();
	
	key = ftok(".", 0);
	size = sizeof(number) + sizeof(int);

	print_numbers();

	for(int i = 0; i < P; i++){
		if(r = pthread_create(&tid [i], NULL, bubble_sort1, NULL) != 0){
			perror("Cannot create multiple threads\n");
		}
	}

	for (int i = 0; i < P; i++) {
		if ((r = pthread_join (tid [i], NULL)) != 0) {
			perror("Cannot join multiple threads\n");
		}
	}
	
	print_numbers();
	fprintf(stderr, "Done.\n");

	gettimeofday(&endtime, NULL);
	fprintf(stderr, "starttime: %ld\n", starttime.tv_usec);
	fprintf(stderr, "endtime: %ld\n", endtime.tv_usec);
	fprintf(stderr, "sorting time: %ldus\n", endtime.tv_usec - starttime.tv_usec);

	return 0;
}