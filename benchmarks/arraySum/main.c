#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#ifdef FLOATS
#define MPI_TYPE MPI_FLOAT
#define C_TYPE float
#else
#define MPI_TYPE MPI_INT
#define C_TYPE int
#endif

#define MATRIX 		0xdead
#define MATRIX2 	0xbeef
#define SUM 		0xf00d
#define READYREQ 	0xbabe

void checkAllocation(void* ptr)
{
	if(!ptr)
	{
		printf("Allocation problem. Exiting...");
		exit(1);	
	}
}

void allocateMemory(C_TYPE ***matrix, int width, int height)
{
	int i;	
	*matrix=(C_TYPE**)malloc(height*sizeof(C_TYPE*));
	for(i=0;i<height;++i)
	{
    	(*matrix)[i]=(C_TYPE*)malloc(width*sizeof(C_TYPE));
    	checkAllocation((*matrix)[i]);
	}
}

void deallocateMemory(C_TYPE ***matrix, int height)
{
	int i;	
	for(i=0;i<height;++i)
	{
    	free((*matrix)[i]);
	}
	free(*matrix);
}

void printMatrix(C_TYPE ***matrix, int width, int height)
{
	int i; int j;
	for(i=0; i< height; ++i)
	{
		for(j=0; j<width; ++j)
		{
			#ifdef FLOATS
			printf("%6.1f ",(*matrix)[i][j]);
			#else
			printf("%4d ",(*matrix)[i][j]);
			#endif
		}
	printf("\n");
	}
}

int main(int argc, char** argv)
{
	struct timeval start, sendend, end, recivstart;
	long secsUsed, microsUsed;

	int seed;
	const int maxWidth = 500;		// width of matrix 
	const int maxHeight = maxWidth; 	// height of matrix 
	C_TYPE **matrix;
	C_TYPE **matrix2;
	C_TYPE **sum;
	int k;
	register int i, j;
	long startTime, endTime, elapsed;
	int times = 1000;
	int myid;
	int processors;

	int maxSubmatrixHeight;

	seed = time(NULL);
	srand(seed);



	printf("Initiating MPI...\n");
	MPI_Init(&argc,&argv);              /* starts MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);  /* get current process id */
    MPI_Comm_size(MPI_COMM_WORLD, &processors);     /* get number of processes */

	if(processors == 0)
	{
		printf("No processors available. Exiting...\n");
		exit(1);
	}

    --processors;	/* node 0 is master node to distribute work only */

	maxSubmatrixHeight = maxHeight / processors;

	if(myid == 0)
	{
		#ifdef FLOATS
		printf("[M]-Running FLOATS test...\n");
		#else
		printf("[M]-Running INT test...\n");
		#endif
    	printf("[M]-Distributing work for %d nodes...\n", processors);
		printf("[M]-Allocating memory...\n");

		allocateMemory(&matrix, maxWidth, maxHeight);
		allocateMemory(&matrix2, maxWidth, maxHeight);
		allocateMemory(&sum, maxWidth, maxHeight);
		checkAllocation(matrix);
		checkAllocation(matrix2);
		checkAllocation(sum);

		printf("[M]-Init matricies...\n");
		for(i=0;i<maxWidth;++i)
		{
			for(j=0;j<maxHeight;++j)
			{
				matrix[i][j] = rand()%100;
				matrix2[i][j] = rand()%100;
			}
		}
		printf("[M]-Starting INT SUMMING...\n");
		gettimeofday(&start, NULL);

		int processor;
		for(processor=1; processor<=processors;++processor)
		{
			printf("[M]-Sending work for %d node...\n", processor);
			int chunk;
			for(chunk = 0; chunk < maxSubmatrixHeight; ++ chunk)
			{
				int row = (processor-1)*maxSubmatrixHeight+chunk;
				MPI_Send(&matrix[row][0],maxWidth, MPI_TYPE, processor, MATRIX, MPI_COMM_WORLD);
				MPI_Send(&matrix2[row][0],maxWidth, MPI_TYPE, processor, MATRIX2, MPI_COMM_WORLD);
			}
		}

		gettimeofday(&sendend, NULL);

		int resultsReceived = 0;

		char receptionStarted = 0;

		while(resultsReceived < processors)
		{
			MPI_Status status;
			MPI_Recv(0, 0, MPI_TYPE, MPI_ANY_SOURCE, READYREQ, MPI_COMM_WORLD, &status);
			if(receptionStarted == 0)
			{
				gettimeofday(&recivstart, NULL);
				receptionStarted = 1;
			}
			int processor = status.MPI_SOURCE; 
			printf("[M]-Received READYREQ from %d node...\n", processor);
			int chunk;
			for(chunk = 0; chunk < maxSubmatrixHeight; ++ chunk)
			{
				int row = (processor-1)*maxSubmatrixHeight+chunk;
				MPI_Recv(&sum[row][0], maxWidth, MPI_TYPE, processor, SUM, MPI_COMM_WORLD, &status);
			}
			++resultsReceived;
		}

		gettimeofday(&end, NULL);
		secsUsed=(end.tv_sec - start.tv_sec);
	    microsUsed= ((secsUsed*1000000) + end.tv_usec) - (start.tv_usec);
		printf("[M]-Time: %lf [seconds]\n", microsUsed/1000000.0);

		long sendTimeSecs = (sendend.tv_sec - start.tv_sec);
		long sendTime = ((sendTimeSecs*1000000) + sendend.tv_usec) - (start.tv_usec);
		printf("[M]-SendTime: %lf [seconds]\n", sendTime/1000000.0);

		long recvTimeSecs = (end.tv_sec - recivstart.tv_sec);
		long recvTime = ((sendTimeSecs*1000000) + end.tv_usec) - (recivstart.tv_usec);
		printf("[M]-RecivTime: %lf [seconds]\n", recvTime/1000000.0);

		deallocateMemory(&matrix,maxHeight);
		deallocateMemory(&matrix2,maxHeight);
		deallocateMemory(&sum,maxHeight);
	}
	else
	{
		printf("[S]-Running as slave %d...\n", myid);

		printf("[S]-Allocating memory...\n");
		allocateMemory(&matrix,maxWidth,maxSubmatrixHeight);
		allocateMemory(&matrix2,maxWidth,maxSubmatrixHeight);
		allocateMemory(&sum,maxWidth,maxSubmatrixHeight);

		checkAllocation(matrix);
		checkAllocation(matrix2);
		checkAllocation(sum);
		printf("[S]-Allocation done...\n");
		MPI_Status status;
		int chunk;
		for(chunk = 0; chunk < maxSubmatrixHeight; ++ chunk)
		{
			MPI_Recv(&matrix[chunk][0], maxWidth, MPI_TYPE, 0, MATRIX, MPI_COMM_WORLD, &status);
			MPI_Recv(&matrix2[chunk][0], maxWidth, MPI_TYPE, 0, MATRIX2, MPI_COMM_WORLD, &status);
		}

		for(k=0;k<times;++k)
		{
			for(i=0;i<maxSubmatrixHeight;++i)
			{
				for(j=0;j<maxWidth;++j)
				{
					sum[i][j] = matrix[i][j] + matrix2[i][j];
				}
			}
		}

		MPI_Send(0, 0, MPI_TYPE, 0, READYREQ, MPI_COMM_WORLD);

		for(chunk = 0; chunk < maxSubmatrixHeight; ++ chunk)
		{
			MPI_Send(&sum[chunk][0],maxWidth, MPI_TYPE, 0, SUM, MPI_COMM_WORLD);
		}
		printf("[S]-Received array...\n");

		deallocateMemory(&matrix,maxSubmatrixHeight);
		deallocateMemory(&matrix2,maxSubmatrixHeight);
		deallocateMemory(&sum,maxSubmatrixHeight);
	}
	MPI_Finalize();     
	return 0;
}