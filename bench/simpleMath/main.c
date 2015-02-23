#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void testMIPS();
void testFLOPS();

int main(int argc, char** argv)
{
	printf("Starting measurements...\n");
	testMIPS();
	testFLOPS();
	return 0;
}

void testMIPS()
{
	struct timeval start, end;
	long secs_used, micros_used;

	int seed;
	const int max_i = 1000;
	const int max_j = max_i;
	char **matrix;
	char **matrix2;
	char **sum;
	int k;
	register int i, j;
	long start_time, end_time, elapsed;
	int times = 1000;

	printf("Seeding...\n");
	seed = time(NULL);
	srand(seed);

	printf("Allocating memory...\n");

	matrix=(char**)malloc(max_i*sizeof(char*));
	matrix2=(char**)malloc(max_i*sizeof(char*));
	sum=(char**)malloc(max_i*sizeof(char*));

	for(j=0;j<max_j;++j)
	{
    	matrix[j]=(char*)malloc(max_i*sizeof(char));
    	matrix2[j]=(char*)malloc(max_i*sizeof(char));
    	sum[j]=(char*)malloc(max_i*sizeof(char));
	}

	printf("Init matricies...\n");
	for(i=0;i<max_i;++i)
	{
		for(j=0;j<max_j;++j)
		{
			matrix[i][j] = rand()%100;
			matrix2[i][j] = rand()%100;
		}
	}
	printf("Starting INT SUMMING...\n");
	gettimeofday(&start, NULL);

	for(k=0;k<times;++k)
	{
		for(i=0;i<max_i;++i)
		{
			for(j=0;j<max_j;++j)
			{
				sum[i][j] = matrix[i][j] + matrix2[i][j];
				sum[j][i] = matrix[i][j] * matrix2[i][j];
			}
		}
	}
	gettimeofday(&end, NULL);
	secs_used=(end.tv_sec - start.tv_sec);
    micros_used= ((secs_used*1000000) + end.tv_usec) - (start.tv_usec);

	printf("Time: %lf [seconds]\n", micros_used/1000000.0);
	printf("MI operations: %i\n", max_i*max_j*times*2);

	printf("MIPS: %lf\n", (max_i*max_j*times*2/(double)(micros_used)));
}

void testFLOPS()
{
	struct timeval start, end;
	long secs_used, micros_used;

	int seed;
	const int max_i = 1000;
	const int max_j = max_i;
	float **matrix;
	float **matrix2;
	float **sum;
	int k;
	register int i, j;
	long start_time, end_time, elapsed;
	int times = 1000;

	printf("Seeding...\n");
	seed = time(NULL);
	srand(seed);

	printf("Allocating memory...\n");

	matrix=(float**)malloc(max_i*sizeof(float*));
	matrix2=(float**)malloc(max_i*sizeof(float*));
	sum=(float**)malloc(max_i*sizeof(float*));

	for(j=0;j<max_j;++j)
	{
    	matrix[j]=(float*)malloc(max_i*sizeof(float));
    	matrix2[j]=(float*)malloc(max_i*sizeof(float));
    	sum[j]=(float*)malloc(max_i*sizeof(float));
	}

	printf("Init matricies...\n");
	for(i=0;i<max_i;++i)
	{
		for(j=0;j<max_j;++j)
		{
			matrix[i][j] = (float)rand()/(float)(RAND_MAX/100.0);
			matrix2[i][j] = (float)rand()/(float)(RAND_MAX/100.0);
		}
	}
	printf("Starting FLOAT SUMMING...\n");
	gettimeofday(&start, NULL);

	for(k=0;k<times;++k)
	{
		for(i=0;i<max_i;++i)
		{
			for(j=0;j<max_j;++j)
			{
				sum[i][j] = matrix[i][j] + matrix2[i][j];
				sum[j][i] = matrix[i][j] * matrix2[i][j];
			}
		}
	}
	gettimeofday(&end, NULL);
	secs_used=(end.tv_sec - start.tv_sec);
    micros_used= ((secs_used*1000000) + end.tv_usec) - (start.tv_usec);

	printf("Time: %lf [seconds]\n", micros_used/1000000.0);
	printf("FLP operations: %i\n", max_i*max_j*times*2);

	printf("MFLOPS: %lf\n", (max_i*max_j*times*2/(double)(micros_used)));
}