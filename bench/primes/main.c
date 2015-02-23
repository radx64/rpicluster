#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void test();

int main(int argc, char** argv)
{
	printf("Starting measurements...\n");
	test();
	return 0;
}

void test()
{
	struct timeval start, end;
	long secs_used, micros_used;

	int i, num = 1, primes = 0;
	int limit = 1000;

	gettimeofday(&start, NULL);

  	for (num = 1; num <= limit; num++)
  	{ 
        int i = 2; 
        while(i <= num)
        { 
        	if(num % i == 0)break;
            i++; 
        }
        if(i == num) primes++;
    }

	gettimeofday(&end, NULL);
	secs_used=(end.tv_sec - start.tv_sec);
    micros_used= ((secs_used*1000000) + end.tv_usec) - (start.tv_usec);
	printf("Time: %lf [seconds]\n", micros_used/1000000.0);
	printf("This machine calculated all %d prime numbers under %d\n",primes,limit);
}