/* Even-Odd Sequential Bubble Sort
 * Author: Kartik Gopalan
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

#define MAX_COUNT 1000000 // Max integers to sort
#define MAX_NUM 100 // Generate numbers between 0 and MAX_NUM

struct thread_data
{
	int start;
	int n;
	int pass;
	int *swap_count_ptr;
};

// Uncomment the following line to turn on debugging messages
//#define DEBUG

long number[MAX_COUNT];
int N; // Number of integers to sort
int P; // Number of processes

// pthread_mutex_t lock;

// generate N random numbers between 0 and MAX_NUM
void generate_numbers()
{
	int i;

	srandom(time(NULL));

	for(i=0; i<N; i++) 
		number[i] = random()%MAX_NUM;
}

void print_numbers() 
{
	int i;

	for(i=0; i<N; i++) 
		printf("%ld ", number[i]);
	printf("\n");
}

int compare_and_swap(int i, int j) 
{
#ifdef DEBUG
	fprintf(stderr,"i %d j %d\n", i, j);
#endif
	assert ( i<N );
	assert ( j<N );

	if( number[i] > number[j]) {
		long temp = number[i];
		number[i] = number[j];
		number[j] = temp;
		return 1;
	}

	return 0;
}

void *bubble(void *thread_args)
{
	struct thread_data *data;
	data = (struct thread_data *) thread_args;

	int start, n, pass, next;
	int *swap_count_ptr;
	start = data->start;
	n = data->n;
	pass = data->pass;
	swap_count_ptr = data->swap_count_ptr;
	next = start;

	assert(start < N-1);

	while ( (next+1) < (start+n) ) {
		*swap_count_ptr += compare_and_swap(next, next+1);
		next+=2;
	}
}

void bubble_sort()
{
	int start = 0;
	int n;
	int pass = 0;
	int number_count, last_count, swap_count = 1;
	pthread_t threads[P];
	struct thread_data thread_args[P];
	int swap_count_thread[P];

	while (last_count != 0 || swap_count != 0)
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
		last_count = swap_count;
		swap_count = 0;
		for (int i = 0; i < P; i++)
		{
			swap_count_thread[i] = 0;
			swap_count = 0;
			n = ((number_count / 2) / P);
			if (((number_count / 2) % P) > i)
				n += 1;
			n *= 2;

			thread_args[i].start = start;
			thread_args[i].n = n;
			thread_args[i].pass = pass;
			thread_args[i].swap_count_ptr = &swap_count_thread[i];

			if (n != 0)
				pthread_create(&threads[i], NULL, bubble, &thread_args[i]);

			start += n;
		}
		for (int i = 0; i < P; i++)
		{
			pthread_join(threads[i], NULL);
			swap_count += swap_count_thread[i];
		}
			
		pass = pass == 0 ? 1 : 0;
	}
}

int main(int argc, char *argv[])
{

	if( argc != 3) {
		fprintf(stderr, "Usage: %s N\n", argv[0]);
		return 1;
	}

	N = strtol(argv[1], (char **)NULL, 10);
	if( (N < 2) || (N > MAX_COUNT) ) {
		fprintf(stderr, "Invalid N. N should be between 2 and %d.\n", MAX_COUNT);
		return 2;
	}

	P = strtol(argv[2], (char **)NULL, 10);

	//fprintf(stderr, "Generating.\n");
	generate_numbers();

	//fprintf(stdout, "Generated sequence is as follows:\n");
	print_numbers();

	//fprintf(stderr, "Sorting.\n");
	bubble_sort();

	//fprintf(stdout, "Sorted sequence is as follows:\n");
	print_numbers();
	fprintf(stderr, "Done.\n");

	return 0;
}