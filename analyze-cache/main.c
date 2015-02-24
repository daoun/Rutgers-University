#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "analyzecache.h"
#include<sys/time.h>

#define BILLION 1000000000L
#define SIZE (16*1024*1024)
#define ITERATIONS 50

char array[SIZE];

//move the time measure outside of the loop
//skip the first access time for it may alter results

int get_cache_line_size()
{
   // struct timespec start, end;
    struct timeval start,end;
   // volatile char x;
    int i,j;
    int increase = 1;
    long int diff=0;
    double ta = 0, temp;
    double total_iter = 0.0;
    
    for(j = 0; j < SIZE; j++)
    {
        //x=array[j];
    }
    
    do
    {
        increase = increase * 2;
        temp = ta;
        ta = 0.0;
        
  //      clock_gettime(CLOCK_MONOTONIC, &start);
        
        //increment the iteration size so that there is same amount of iterations
        gettimeofday(&start, 0);
        
        for(i = 0; i < ITERATIONS*increase  ; i ++)
        {
            for(j = 0; j < SIZE; j=j+increase)
            {
                array[j]='a';
                //array[(j * increase)%SIZE]='a';
            }
        }
        gettimeofday(&end, 0);
    //    clock_gettime(CLOCK_MONOTONIC, &end);
        
       // diff = BILLION * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
        diff=(end.tv_sec * 1000000 + end.tv_usec)
        - (start.tv_sec * 1000000 + start.tv_usec);
        total_iter = ( (double)(diff*increase) ) / ( (double)(SIZE) );
        ta = total_iter / ITERATIONS;
        
        printf("cl : avg time accessed = %lf ", ta);
        printf("and time elapsed = %llu ", diff);
        printf("for %dB\n", increase);
        diff=0;
        
    }while(increase < 256);
    
    return (increase / 2);
}

int get_cache_size(int clsize)
{
    struct timespec start, end;
    
    volatile char x;
    int i,j, increase = (clsize/2);
    uint64_t diff;
    double temp, ta = 0.0, total_iter = 0.0;
    
    do
    {
        increase = increase * 2;
        temp = ta;
        ta = 0.0;
        
        
        for(j = 0; j < SIZE; j = j + increase)
        {
            x=array[j];
        }
        
 //       clock_gettime(CLOCK_MONOTONIC, &start);
        
        for(i = 0; i < ITERATIONS; i++)
        {
            for(j = 0; j < SIZE; j = j + increase)
            {
                x=array[j];
            }
        }
        
    //    clock_gettime(CLOCK_MONOTONIC, &end);
        
        diff = BILLION * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
        total_iter = ( (double)(diff * increase) ) / ( (double)(SIZE) );
        ta = total_iter / ITERATIONS;
        
        printf("c : avg time accessed = %lf ", ta);
        printf("and time elapsed = %llu ", diff);
        printf("for %dB\n", increase);
        
        
        
    }while(increase < SIZE);
    
    return (increase / 2);
}

double get_miss_penalty(int clsize)
{
    struct timespec start, end;
    
    volatile char x;
    int m, i, j, increase = clsize, iter = ITERATIONS;
    uint64_t ta[3];
    double tmiss;
    
    for(m = 0; m < 3; m++)
    {
        //	for(j = 0; j < SIZE; j = j + increase)
        //	{
        //		x=array[j];
        //	}
        
     //   clock_gettime(CLOCK_MONOTONIC, &start);
        for(i = 0; i < (iter * m); i++)
        {
            for(j = 0; j < SIZE; j = j + increase)
            {
                x=array[j];
            }
        }
   //     clock_gettime(CLOCK_MONOTONIC, &end);
        ta[m] = BILLION * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
    }
    
    printf("tmiss : ta0 = %llu, ta1 = %llu, ta2 = %llu \n", ta[0], ta[1], ta[2]);
    
    tmiss = (double)(ta[2] - ta[1]) / 
    (double)( (SIZE / 64)*(iter * 3) - (SIZE / 64)*(iter * 2) );
    
    return tmiss;
}

int main(int argc, char *argv[])
{
    int cache_line_size;
    int cache_size;
    double miss_penalty;
    
    int cl_size = 64;
    
    cache_line_size = get_cache_line_size();
    printf("Cache Block/Line Size: %d B\n", cl_size);
    
   // cache_size = get_cache_size(cl_size);
    //printf("Cache Size: %d KB\n", cache_size);
    
    //miss_penalty = get_miss_penalty(cl_size);
    //printf("Cache Miss Penalty: %lf us\n", miss_penalty);
    
    return 0;
}

