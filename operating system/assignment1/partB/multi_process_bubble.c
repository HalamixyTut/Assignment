/* Even-Odd Sequential Bubble Sort
 * Author: Kartik Gopalan
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/unistd.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <errno.h>
#include <wait.h>
#include <string.h>

#define MAX_COUNT 1000000 // Max integers to sort
#define MAX_NUM 100 // Generate numbers between 0 and MAX_NUM

// Uncomment the following line to turn on debugging messages
//#define DEBUG

long number[MAX_COUNT];
int N; // Number of integers to sort
int P; // Number of processes

key_t key;
size_t size;

int shmid;
int pid;

long *shm;
int *swap_count;

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

	if( shm[i] > shm[j]) {
		long temp = shm[i];
		shm[i] = shm[j];
		shm[j] = temp;
		return 1;
	}

	return 0;
}

// even-odd pass bubbling from start to start+n
int bubble(int start, int n, int pass) 
{
#ifdef DEBUG
	fprintf(stderr, "start %d n %d pass %d\n", start, n, pass);
#endif

	int next = start;

	assert (start < N-1); // bug if we start at the end of array

	while ( (next+1) < (start+n) ) {
		*swap_count += compare_and_swap(next, next+1);
		next+=2;
	}
}

void bubble_sort()
{
	int start = 0;
	int n;
	int pass = 0;
	int number_count, last_count;
	
	while (last_count != 0 || *swap_count != 0)
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
		last_count = *swap_count;
		*swap_count = 0;
		for (int i = 0; i < P; i++)
		{
			n = ((number_count / 2) / P);
			if (((number_count / 2) % P) > i)
				n += 1;
			n *= 2;

			if (n != 0)
				pid = fork();
			if (pid == 0)
				break;
			start += n;
		}
		if (pid == 0)
		{
			// printf("%d,%d,%d\n",start, n, getpid());
			bubble(start, n, 0);
			exit(0);
		}
		waitpid(pid,0,0);
		// wait(0);
		pass = pass == 0 ? 1 : 0;
	}
}


int main(int argc, char *argv[])
{

	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	
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

	key = ftok(".", 0);
	size = sizeof(number) + sizeof(int);

	shmid = shmget(key, size, IPC_CREAT | 0644);
	shm = (long*)shmat(shmid, NULL, 0);
	swap_count = (int*)(shm + MAX_COUNT);

	memcpy(shm, number, sizeof(number));
	// *swap_count = N;

	//fprintf(stdout, "Generated sequence is as follows:\n");
	print_numbers();

	//fprintf(stderr, "Sorting.\n");
	bubble_sort();

	//fprintf(stdout, "Sorted sequence is as follows:\n");
	memcpy(number, shm, sizeof(number));
	print_numbers();
	fprintf(stderr, "Done.\n");

	shmdt(shm);
	
	gettimeofday(&end, NULL);
        fprintf(stderr, "starttime: %ld\n", start.tv_usec);
        fprintf(stderr, "endtime: %ld\n", end.tv_usec);
        fprintf(stderr, "sorting time: %ldus\n", end.tv_usec-start.tv_usec);
	
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

	return 0;
}

