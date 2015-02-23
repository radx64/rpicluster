#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

#define WORK		1
#define RESULT		2
#define WANTMORE	3
#define DIE			4
#define END			5

void manager(int numOfWorkers);
void worker(int id);

int main(int argc, char** argv)
{
	int id, numOfAllProcessors;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfAllProcessors);
	if(id == 0) manager(numOfAllProcessors-1);
	else worker(id);
	MPI_Finalize();
	return 0;
}

void manager(int numOfWorkers)
{
	struct timeval start, end;
	long secs_used, micros_used;
	int limit = 20000;
	int primes = 0;
	int worker;
	printf("Starting processes manager...\n");
	gettimeofday(&start, NULL);

	for(worker = 1; worker<=numOfWorkers; ++worker)
	{
		int part = worker - 1;
		int lowerBound = part * limit/numOfWorkers + 1;
		int higherBound = (part+1) * limit/numOfWorkers;
		int range[2];
		range[0] = lowerBound;
		range[1] = higherBound;
		MPI_Send(range, 2, MPI_INT, worker, WORK, MPI_COMM_WORLD);
	}
	for(worker = 1; worker<=numOfWorkers; ++worker)
	{
		int result;
		MPI_Status status;
		MPI_Recv(&result, 1, MPI_INT, worker, RESULT, MPI_COMM_WORLD, &status);
		primes += result;
	}
	gettimeofday(&end, NULL);
	secs_used=(end.tv_sec - start.tv_sec);
    micros_used= ((secs_used*1000000) + end.tv_usec) - (start.tv_usec);
	printf("Time: %lf [seconds]\n", micros_used/1000000.0);
	printf("This cluster calculated all %d prime numbers in range [1:%d]\n",primes,limit);
}

void worker(int id)
{
	printf("Starting worker %d ...\n", id);
	int i, num, primes = 0;

	MPI_Status status;
	int range[2];
	MPI_Recv(range, 2, MPI_INT, 0, WORK, MPI_COMM_WORLD, &status);
	int lowerBound = range[0];
	int higherBound = range[1];

	printf("Worker %d has range [%d:%d]\n", id, lowerBound, higherBound);

  	for (num = lowerBound; num <= higherBound; num++)
  	{ 
        int i = 2; 
        while(i <= num)
        { 
        	if(num % i == 0)break;
            i++; 
        }
        if(i == num) primes++;
    }
    MPI_Send(&primes, 1, MPI_INT, 0, RESULT, MPI_COMM_WORLD);
}