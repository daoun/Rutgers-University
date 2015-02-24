#ifndef H_ANALYZE_CACHE
#define H_ANALYZE_CACHE

// Your header code goes here, between #define and #endif

int get_cache_line_size();
int get_cache_size(int);


/* use the size of cacheline 
 * iterate many times
 * iterate through an array bigger than the cacheline size
 * calculate the time elapsed and get the constant time that takes for the
 * 		program to calculate the time and subtract it from elapsed time
* */
double get_miss_penalty(int);

#endif

