#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "analyzecache.h"

#define MILLION 1000000L
#define SIZE (16*1024*1024)
#define ITERATIONS 50

char array[SIZE];

//move the time measure outside of the loop
//skip the first access time for it may alter results

int get_cache_line_size()
{
	struct timeval start, end;
		
	volatile char x;
	int i,j;
	int increase = 1, check = 0;
	long int diff;
	double ta = 0, temp1 = 1, temp2 = 0;
	double total_iter = 0.0;	
	
	for(j = 0; j < SIZE; j++)
	{
		x=array[j];
		//array[j] = 'a';
	}
	
	do
	{
		increase = increase * 2;
		temp2 = temp1;
		
		gettimeofday(&start, 0);
		
		//increment the iteration size so that there is same amount of iterations 
		
		
		for(i = 0; i < ITERATIONS  ; i ++)
		{
			for(j = 0; j < SIZE; j = j + increase)
			{
				x=array[(j * increase)%SIZE];
				//array[(j * increase)%SIZE] = 'a';
				
			}	
		}
		
		gettimeofday(&end, 0);
			
		diff = MILLION * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
		total_iter = ( (double)(diff * increase) ) / ( (double)(SIZE) );
		ta = total_iter / ITERATIONS;
		
	//	printf("cl : avg time accessed = %lf ", ta);
	//	printf("and time elapsed = %llu ", diff);
	//	printf("for %dB\n", increase);
		
		temp1 = 1000 * ta;
		
		if(temp1 > (temp2 * 2))
			check = 1;
		if(check == 1 && (int)(temp1/1) == (int)(temp2/1))
			check = 2;
		
	}while(check != 2);
	
	return (increase / 2);
}

int get_cache_size(int clsize)
{
	struct timeval start, end;
		
	volatile char x;
	int i,j, check = 0, increase = (clsize/2);
	uint64_t diff;
	double temp1 = 1, temp2 = 0, ta = 0.0, total_iter = 0.0;	
	
	for(j = 0; j < SIZE; j++)
	{
		x=array[j];
	}
	
	do
	{
		increase = increase * 2;
		temp2 = temp1;
		
		gettimeofday(&start, 0);
		
		for(i = 0; i < ITERATIONS; i++)
		{
			for(j = 0; j < SIZE; j = j + increase)
			{
				x=array[j];
			}	
		}
		
		gettimeofday(&end, 0);
			
		diff = MILLION * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
		total_iter = ( (double)(diff * increase) ) / ( (double)(SIZE) );
		ta = total_iter / ITERATIONS;
		
	//	printf("c : avg time accessed = %lf ", ta);
	//	printf("and time elapsed = %llu ", diff);
	//	printf("for %dB\n", increase);
		
		temp1 = 1000 * ta;
		
		if(temp1 > (temp2 * 2))
		{
			check = 1;
		}
		if(check == 1 && (int)(temp1/1) == (int)(temp2/1))
		{
			check = 2;
		}
		
	}while(check != 2);
	
	return (increase / 1024);
}

double get_miss_penalty(int clsize)
{
	struct timeval start, end;
		
	volatile char x;
	int m, i, j, increase = clsize, iter = ITERATIONS;
	long int ta[3];
	double tmiss;
	
	for(m = 1; m < 4; m++)
	{
		gettimeofday(&start, 0);
		for(i = 0; i < (iter * m); i++)
		{
			for(j = 0; j < SIZE; j = j + increase)
			{
				x=array[j];
			}	
		}
		gettimeofday(&end, 0);
		ta[m] = MILLION * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
	}
	
	//printf("tmiss : ta0 = %llu, ta1 = %llu, ta2 = %llu \n", ta[0], ta[1], ta[2]);
	
	tmiss = (double)(ta[2] - ta[1]) / 
		(double)( (SIZE / increase)*(iter * 3) - (SIZE / increase)*(iter * 2) );
	
	return tmiss;
}

int main(int argc, char *argv[])
{
	int cache_line_size;
	int cache_size;
	double miss_penalty;
	
	printf("********* H W 0 - C A C H E *********\n");
	
	cache_line_size = get_cache_line_size();
	printf("Cache Block/Line Size: %d B\n", cache_line_size);
	
	cache_size = get_cache_size(cache_line_size);
	printf("Cache Size: %d KB\n", cache_size);
	
	miss_penalty = get_miss_penalty(cache_line_size);
	printf("Cache Miss Penalty: %lf us\n", miss_penalty);
	
	printf("*************************************\n");
	
	return 0;
}


