#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

#define WORK        1
#define RESULT      2
#define END         3

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
    int limit = 100000;
    int numbersInPack = 5000;
    int numOfPacksToSend = limit / numbersInPack;
    int actualPackToSend = 0;
    int primes = 0;
    int worker;
    printf("[M]-Starting process manager...\n");
    gettimeofday(&start, NULL);

    for(worker = 1; worker<=numOfWorkers; ++worker)
    {
        int part = worker - 1;
        int lowerBound = actualPackToSend * numbersInPack + 1;
        int higherBound = (actualPackToSend+1) * numbersInPack;
        int range[2];
        range[0] = lowerBound;
        range[1] = higherBound;
        printf("Sending pack %d of %d\n", actualPackToSend,numOfPacksToSend);
        MPI_Send(range, 2, MPI_INT, worker, WORK, MPI_COMM_WORLD);
        actualPackToSend++;
    }

    while(actualPackToSend <= numOfPacksToSend)
    {
        int result;
        MPI_Status status;
        MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, RESULT, MPI_COMM_WORLD, &status);
        int worker = status.MPI_SOURCE;
        primes += result;
        int lowerBound = actualPackToSend * numbersInPack + 1;
        int higherBound = (actualPackToSend+1) * numbersInPack;
        int range[2];
        range[0] = lowerBound;
        range[1] = higherBound;
        printf("Sending pack %d of %d\n", actualPackToSend,numOfPacksToSend);
        MPI_Send(range, 2, MPI_INT, worker, WORK, MPI_COMM_WORLD);
        actualPackToSend++;
    }

    for(worker = 1; worker<=numOfWorkers; ++worker)
    {
        int result;
        MPI_Status status;
        MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, RESULT, MPI_COMM_WORLD, &status);
        int worker = status.MPI_SOURCE;
        primes += result;   
    }

    for(worker = 1; worker<=numOfWorkers; ++worker)
    {
        MPI_Send(0, 0, MPI_INT, worker, END, MPI_COMM_WORLD);
    }
    gettimeofday(&end, NULL);
    secs_used=(end.tv_sec - start.tv_sec);
    micros_used= ((secs_used*1000000) + end.tv_usec) - (start.tv_usec);
    printf("Time: %lf [seconds]\n", micros_used/1000000.0);
    printf("This cluster calculated all %d prime numbers in range [1:%d]\n", primes ,limit);
}

void worker(int id)
{
    printf("[S]-Starting worker %d ...\n", id);
    int i, num;
    
    while(1)
    {
        int primes = 0;
        MPI_Status status;
        int range[2];
        MPI_Recv(&range, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        int tag = status.MPI_TAG;
        if (tag == END) break;

        int lowerBound = range[0];
        int higherBound = range[1];

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
        printf("[%d]In range %d : %d got %d primes.\n",id, lowerBound, higherBound, primes);

        MPI_Send(&primes, 1, MPI_INT, 0, RESULT, MPI_COMM_WORLD);
    }

}